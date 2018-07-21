#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include <QCoreApplication>
#include <Qdir>

#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/prm/LazyPRM.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/rrt/LazyRRT.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/stride/STRIDE.h>
#include <omplapp/apps/SE3RigidBodyPlanning.h>
#include <omplapp/config.h>

using namespace ompl;

std::ifstream fptr_cfg_list;
std::ifstream fptr_cfg;
std::ofstream fptr_experiement_data;

struct Quaterniond {
  double x, y, z, w;
};

struct CFG {
  double tx, ty, tz;  // translation
  double rx, ry, rz;  // rotation
  Quaterniond quaternion;
};

std::string working_dir;
std::string input_dir = "inputs/WAFR2018/";
std::vector<std::string> cfg_list;
std::string cfg_list_name = "cfg_list.txt";
std::string cfg_name;
struct CFG start, goal;
std::string robot;
std::string world;
double time_limit = 60;  // 60 s
int run_count = 10;      // 10 rounds

struct Data {
  double num;
  double total_time, avg_time, std_time, best_time;
  double success_rate;
  std::vector<double> t;
  Data(double n, double tt, double at, double st, double sr)
      : num(n), total_time(tt), avg_time(at), std_time(st), success_rate(sr) {
    t.clear();
  }
};

void parseExampleFile();
void run(const int planner_idx);

void pwdWorkingDir(std::string argv) {
  bool found_dir = false;
  int idx = 0;
  for (int i = argv.size() - 1; i >= 0; --i) {
    if (argv[i] == '/') {
      idx = i + 1;
      break;
    }
  }
  std::string folder_name = "/" + argv.substr(idx, argv.size()) + "/";
  working_dir = QDir::currentPath().toStdString();

  // Test if the build directory is project_name. If so,
  // the path to the current working directory will
  // include /project_name
  size_t idx_dir = working_dir.rfind(folder_name);
  if (idx_dir != std::string::npos) {
    working_dir = working_dir.substr(0, idx_dir + folder_name.size());
    // Set current working directory to project_name
    QDir::setCurrent(working_dir.c_str());
    found_dir = true;
  }

  // Test if a build directory (/build-project_name...) was created. This
  // directory will reside in the same directory as /project_name
  if (!found_dir && (idx_dir = working_dir.rfind("build-" + folder_name)) !=
                        std::string::npos) {
    QDir profile(working_dir.substr(0, idx_dir).c_str());

    if (profile.exists(
            std::string(folder_name + "/" + folder_name + ".pro").c_str())) {
      working_dir = working_dir.substr(0, idx_dir) + "/" + folder_name;

      // Set current working directory to project_name
      QDir::setCurrent(working_dir.c_str());
      found_dir = true;
    }
  }

  // /project_name could not be found
  if (!found_dir) {
    std::cerr << std::endl
              << "!! WARNING !!\n"
              << "The program may not work correctly.\n"
              << std::endl;
  }
}

int main(int argc, char** argv) {
  // set up working directory
  pwdWorkingDir(std::string(argv[0]));
  working_dir = working_dir + input_dir;

  // read cfg list
  fptr_cfg_list.open(working_dir + cfg_list_name);
  if (!fptr_cfg_list.is_open()) {
    std::cerr << "cfg list not found!\n";
  } else {
    while (std::getline(fptr_cfg_list, cfg_name)) {
      cfg_list.push_back(cfg_name);
    }
    fptr_cfg_list.close();
    for (const auto& cfg_name : cfg_list) {
      fptr_cfg.open(working_dir + cfg_name);
      if (!fptr_cfg.is_open()) {
        std::cerr << cfg_name << " cannot open!\n";
      } else {
        parseExampleFile();

        fptr_experiement_data.open(std::string(cfg_name + "_exp.txt"),
                                   std::ofstream::app);
        for (int i = 0; i < 5; ++i)  // planner idx
        {
          // run(i);
        }
        fptr_experiement_data.close();
        fptr_cfg.close();
      }
    }
  }

  return 0;
}

void cube2sphere(double& rx, double& ry, double& rz) {
  double xx = rx;
  double yy = ry;
  double zz = rz;
  rx = xx * sqrt(1.0 - yy * yy * 0.5 - zz * zz * 0.5 + yy * yy * zz * zz / 3.0);
  ry = yy * sqrt(1.0 - xx * xx * 0.5 - zz * zz * 0.5 + xx * xx * zz * zz / 3.0);
  rz = zz * sqrt(1.0 - xx * xx * 0.5 - yy * yy * 0.5 + xx * xx * yy * yy / 3.0);
}

Quaterniond toQuaternion(double x, double y, double z) {
  // 1. Normalize
  cube2sphere(x, y, z);

  // 2. Cross Product with North Pole
  float qx = -z;
  float qy = 0;
  float qz = x;
  float qnorm = sqrt(qx * qx + qy * qy + qz * qz);
  if (qx == 0 && qy == 0 && qz == 0) {
    qx = 1;
    qz = 0;
    qnorm = 1;
  }
  qx /= qnorm;
  qy /= qnorm;
  qz /= qnorm;

  // 3. Angle with North Pole
  float theta = acos(y);

  // 4. Get Quaternion
  Quaterniond q;
  float c = cos(theta / 2);
  float s = sin(theta / 2);
  double w = q.w = c;
  x = q.x = s * qx;
  y = q.y = s * qy;
  z = q.z = s * qz;
  fprintf(stderr, "xyzw %f %f %f %f  norm %f\n", q.x, q.y, q.z, q.w, qnorm);
  // roll (x-axis rotation)
  double sinr = +2.0 * (q.w * q.x + q.y * q.z);
  double cosr = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
  double roll = atan2(sinr, cosr);

  // pitch (y-axis rotation)
  double sinp = +2.0 * (q.w * q.y - q.z * q.x);
  double pitch = asin(sinp);

  // yaw (z-axis rotation)
  double siny = +2.0 * (q.w * q.z + q.x * q.y);
  double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
  double yaw = atan2(siny, cosy);

  fprintf(stderr, "r p y %f %f %f\n", roll * 180.0 / M_PI, pitch * 180.0 / M_PI,
          yaw * 180.0 / M_PI);

  return q;
}

void adjust_OMPL_translation_input(double& tx, double& ty, double& tz,
                                   double rx, double ry, double rz) {
  cube2sphere(rx, ry, rz);

  std::string num = "";
  for (unsigned i = 0; i < robot.size(); ++i) {
    if (robot[i] == '.') {
      break;
    }
    num.append(1, robot[i]);
  }
  double rod_length = atof(num.c_str()) * 0.5;

  double norm = sqrt(rx * rx + ry * ry + rz * rz);
  if (norm != 0) {
    tx += rx / norm * rod_length;
    ty += ry / norm * rod_length;
    tz += rz / norm * rod_length;
  }
}

void parseExampleFile() {
  std::string tmp_string;
  char tmp_char[1000];
  while (std::getline(fptr_cfg, tmp_string)) {
    strcpy(tmp_char, tmp_string.c_str());
    char* sptr = strtok(tmp_char, "=: \t");

    // comments
    if (sptr == nullptr || strcmp(sptr, "#") == 0) {
      continue;
    }
    fprintf(stderr, "%s\n", sptr);

    // start configuration
    if (strcmp(sptr, "start.x") == 0) {
      sptr = strtok(NULL, "=: \t");
      start.tx = atof(sptr);
    }
    if (strcmp(sptr, "start.y") == 0) {
      sptr = strtok(NULL, "=: \t");
      start.ty = atof(sptr);
    }
    if (strcmp(sptr, "start.z") == 0) {
      sptr = strtok(NULL, "=: \t");
      start.tz = atof(sptr);
    }
    if (strcmp(sptr, "start.rx") == 0) {
      sptr = strtok(NULL, "=: \t");
      start.rx = atof(sptr);
    }
    if (strcmp(sptr, "start.ry") == 0) {
      sptr = strtok(NULL, "=: \t");
      start.ry = atof(sptr);
    }
    if (strcmp(sptr, "start.rz") == 0) {
      sptr = strtok(NULL, "=: \t");
      start.rz = atof(sptr);
    }

    // goal configuration
    if (strcmp(sptr, "goal.x") == 0) {
      sptr = strtok(NULL, "=: \t");
      goal.tx = atof(sptr);
    }
    if (strcmp(sptr, "goal.y") == 0) {
      sptr = strtok(NULL, "=: \t");
      goal.ty = atof(sptr);
    }
    if (strcmp(sptr, "goal.z") == 0) {
      sptr = strtok(NULL, "=: \t");
      goal.tz = atof(sptr);
    }
    if (strcmp(sptr, "goal.rx") == 0) {
      sptr = strtok(NULL, "=: \t");
      goal.rx = atof(sptr);
    }
    if (strcmp(sptr, "goal.ry") == 0) {
      sptr = strtok(NULL, "=: \t");
      goal.ry = atof(sptr);
    }
    if (strcmp(sptr, "goal.rz") == 0) {
      sptr = strtok(NULL, "=: \t");
      goal.rz = atof(sptr);
    }

    if (strcmp(sptr, "robot") == 0) {
      sptr = strtok(NULL, "=: #\t\n");
      robot = sptr;
      fprintf(stderr, "robot %s\n", sptr);
    }
    if (strcmp(sptr, "world") == 0) {
      sptr = strtok(NULL, "=: #\t\n");
      world = sptr;
      fprintf(stderr, "world %s\n", sptr);
    }

    if (strcmp(sptr, "time_limit") == 0) {
      sptr = strtok(NULL, "=: #\t\n");
      time_limit = atof(sptr);
    }
    if (strcmp(sptr, "run_count") == 0) {
      sptr = strtok(NULL, "=: #\t\n");
      run_count = atoi(sptr);
    }
  }
  adjust_OMPL_translation_input(start.tx, start.ty, start.tz, start.rx,
                                start.ry, start.rz);
  start.quaternion = toQuaternion(start.rx, start.ry, start.rz);
  double qw2 = sqrt(1.0 - start.quaternion.w * start.quaternion.w);
  if (qw2 == 0) qw2 = 1;
  fprintf(stderr, "start translation %f %f %f\n", start.tx, start.ty, start.tz);
  fprintf(stderr, "start rotation %f %f %f %f\n", start.quaternion.x / qw2,
          start.quaternion.y / qw2, start.quaternion.z / qw2,
          2.0 * acos(start.quaternion.w));
  adjust_OMPL_translation_input(goal.tx, goal.ty, goal.tz, goal.rx, goal.ry,
                                goal.rz);
  goal.quaternion = toQuaternion(goal.rx, goal.ry, goal.rz);
  qw2 = sqrt(1.0 - goal.quaternion.w * goal.quaternion.w);
  if (qw2 == 0) qw2 = 1;
  fprintf(stderr, "goal translation %f %f %f\n", goal.tx, goal.ty, goal.tz);
  fprintf(stderr, "goal rotation %f %f %f %f\n", goal.quaternion.x / qw2,
          goal.quaternion.y / qw2, goal.quaternion.z / qw2,
          2.0 * acos(goal.quaternion.w));
}

void run(const int planner_idx) {
  // plan in SE3
  app::SE3RigidBodyPlanning setup;

  // load the robot and the environment
  setup.setRobotMesh(std::string(working_dir + robot).c_str());
  setup.setEnvironmentMesh(std::string(working_dir + world).c_str());

  struct Data experiment(run_count, 0, 0, 0, 0);
  for (int i = 0; i < run_count; ++i) {
    // define starting state
    base::ScopedState<base::SE3StateSpace> m_start(setup.getSpaceInformation());
    m_start->setXYZ(start.tx, start.ty, start.tz);
    double qw2 = sqrt(1.0 - start.quaternion.w * start.quaternion.w);
    m_start->rotation().setAxisAngle(
        start.quaternion.x / qw2, start.quaternion.y / qw2,
        start.quaternion.z / qw2, 2.0 * acos(start.quaternion.w));

    // define goal state
    base::ScopedState<base::SE3StateSpace> m_goal(m_start);
    m_goal->setXYZ(goal.tx, goal.ty, goal.tz);
    qw2 = sqrt(1.0 - goal.quaternion.w * goal.quaternion.w);
    m_goal->rotation().setAxisAngle(
        goal.quaternion.x / qw2, goal.quaternion.y / qw2,
        goal.quaternion.z / qw2, 2.0 * acos(goal.quaternion.w));

    // set the start & goal states
    setup.setStartAndGoalStates(m_start, m_goal);
    // choose different sampling method
    ompl::base::SpaceInformationPtr si_ptr = setup.getSpaceInformation();

    if (planner_idx == 0) {
      fptr_experiement_data << "planner: prm\n";
      ompl::geometric::PRM* prm = new ompl::geometric::PRM(si_ptr);
      prm->setMaxNearestNeighbors(15);
      ompl::base::PlannerPtr target_planner_ptr(prm);
      setup.setPlanner(target_planner_ptr);
    } else if (planner_idx == 1) {
      fptr_experiement_data << "planner: lazy prm\n";
      ompl::base::PlannerPtr target_planner_ptr(
          new ompl::geometric::LazyPRM(si_ptr));
      setup.setPlanner(target_planner_ptr);
    } else if (planner_idx == 2) {
      fptr_experiement_data << "planner: rrt\n";
      ompl::geometric::RRT* rrt = new ompl::geometric::RRT(si_ptr);
      rrt->setGoalBias(0.01);
      rrt->setRange(0.5);
      ompl::base::PlannerPtr target_planner_ptr(rrt);
      setup.setPlanner(target_planner_ptr);
    } else if (planner_idx == 3) {
      fptr_experiement_data << "planner: lazy rrt\n";
      ompl::geometric::LazyRRT* lazyrrt = new ompl::geometric::LazyRRT(si_ptr);
      ompl::base::PlannerPtr target_planner_ptr(lazyrrt);
      setup.setPlanner(target_planner_ptr);
    } else if (planner_idx == 4) {
      fptr_experiement_data << "planner: rrt connect\n";
      ompl::geometric::RRTConnect* rrtconnect =
          new ompl::geometric::RRTConnect(si_ptr);
      rrtconnect->setRange(0.5);
      ompl::base::PlannerPtr target_planner_ptr(rrtconnect);
      setup.setPlanner(target_planner_ptr);
    }

    // Jul. 16 Tom
    // not a good way for evaluating since it is an approximate path ...
    // if(setup.solve()){
    //    suc++;
    //}

    // Jul. 20 Tom
    bool solved = setup.solve(time_limit);
    if (solved && setup.haveExactSolutionPath()) {
      // if(solved && setup.haveSolutionPath()){
      ++experiment.success_rate;
    }

    experiment.total_time += setup.getLastPlanComputationTime() * 1000.0f;
    experiment.t.push_back(setup.getLastPlanComputationTime() * 1000.0f);
    if (experiment.t.size() == 1) {
      experiment.best_time = experiment.t.back();
    } else {
      experiment.best_time =
          std::min(experiment.best_time, experiment.t.back());
    }
  }

  experiment.avg_time = experiment.total_time / experiment.num;
  for (const auto& t : experiment.t) {
    experiment.std_time +=
        (t - experiment.avg_time) * (t - experiment.avg_time);
  }
  experiment.std_time = sqrt(experiment.std_time / experiment.num);
  experiment.success_rate = experiment.success_rate / experiment.num;
  fptr_experiement_data << "avg time: " << experiment.avg_time << std::endl;
  fptr_experiement_data << "std time: " << experiment.std_time << std::endl;
  fptr_experiement_data << "best time: " << experiment.best_time << std::endl;
  fptr_experiement_data << "success rate: " << experiment.success_rate
                        << std::endl;
}
