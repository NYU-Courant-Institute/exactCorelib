#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip> 
#include <cstdlib> 
#include "capd/capdlib.h"
#include "Extend-new.h"
#include "EndEnc.h"

using namespace capd;
using namespace std;





/*

void computeAndPrintCoefficients(
	ICnTaylor& solver, const IVector& B0, int degree) {
	try {
        CnMultiMatrixRect2Set set(B0, degree - 1);
        set.move(solver);
        for (int i = 0; i < B0.dimension(); ++i) {
            std::cout << "Variable x" << i << " coefficients:\n";
            for (int j = 0; j <= degree; ++j) {
                std::cout << "  t^" << j << ": " << solver.coefficient(i, j) << "\n";
            }
        }
	}
	catch (const std::exception& e) {
        std::cerr << "Error in computeAndPrintCoefficients: " << e.what() << std::endl;
	}
}//computeAndPrintCoefficients(

*/


std::ostream& operator<<(std::ostream& os, const std::vector<double>& vec) {
    os << "{ ";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1) {
            os << ", ";  // 仅在不是最后一个元素时加逗号
        }
    }
    os << " }";
    return os;
}


// 解析类似 return1.calD.T[3] 这样的字符串
bool parseIndex(const string& input, string& varName, int& index) {
    size_t start = input.find('[');
    size_t end = input.find(']');
    if (start != string::npos && end != string::npos && end > start) {
        varName = input.substr(0, start);
        string indexStr = input.substr(start + 1, end - start - 1);
        index = stoi(indexStr);
        return true;
    }
    return false;
}

// Interactive shell, allowing users to freely query the value of return1
void interactiveShell(Stage& S) {
    string command;
    cout << "Entering interactive mode, input variable names, for example: S.T,mu1,mu2..., type 'exit' to quit.\n";

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command == "exit") {
            cout << "Exiting interactive mode.\n";
            break;
        }

        int index = -1;
        string varName;
        bool hasIndex = parseIndex(command, varName, index);

        // Query return1.calD.T
        if (command == "S.T") {
            cout << "Please enter the index value (input a negative number to view all): ";
            int userIndex;
            cin >> userIndex;
            cin.ignore();  // Handle newline character to prevent affecting the next input

            if (userIndex < 0) {  // Output all
                for (double t : S.T)
                    cout << t << " ";
                cout << endl;
            }
            else if (userIndex >= 0 && userIndex < S.T.size()) {  // Output specific index
                cout << S.T[userIndex] << endl;
            }
            else {
                cout << "Index out of range\n";
            }
        }

        if (command == "S.E") {
            cout << "Please enter the index value (input a negative number to view all): ";
            int userIndex;
            cin >> userIndex;
            cin.ignore();  // Handle newline character to prevent affecting the next input

            if (userIndex < 0) {  // Output all
                for (IVector t : S.E)
                    cout << t << " ";
                cout << endl;
            }
            else if (userIndex >= 0 && userIndex < S.E.size()) {  // Output specific index
                cout << S.E[userIndex] << endl;
            }
            else {
                cout << "Index out of range\n";
            }
        }




        if (command == "S.F") {
            cout << "Please enter the index value (input a negative number to view all): ";
            int userIndex;
            cin >> userIndex;
            cin.ignore();  // Handle newline character to prevent affecting the next input

            if (userIndex < 0) {  // Output all
                for (IVector t : S.F)
                    cout << t << " ";
                cout << endl;
            }
            else if (userIndex >= 0 && userIndex < S.F.size()) {  // Output specific index
                cout << S.F[userIndex] << endl;
            }
            else {
                cout << "Index out of range\n";
            }
        }

        // Query return1.calQ.mu
        if (command == "mu1") {
            // 检查S.G是否为空
            if (S.G.empty()) {
                cout << "S.G is empty. Nothing to display.\n";
            }
            else {
                // 提示输入i，告知范围
                cout << "Please enter the i index [0, " << S.G.size() - 1 << "], or negative for all: ";
                int i_index;
                cin >> i_index;
                cin.ignore();  // 处理换行符

                if (i_index < 0) {
                    // 输出所有组
                    for (int i = 0; i < S.G.size(); i++) {
                        cout << "Group " << i << ":\n";
                        if (S.G[i].mu1.empty()) {
                            cout << "  (no mu1 data in this group)\n";
                        }
                        else {
                            // 输出该组所有j的mu1
                            for (int j = 0; j < S.G[i].mu1.size(); j++) {
                                cout << "  j = " << j << ": ";
                                for (double val : S.G[i].mu1) {
                                    cout << val << " ";
                                }
                                cout << endl;
                            }
                        }
                    }
                }
                else if (i_index >= S.G.size()) {
                    cout << "i index out of range. Valid range: 0 to " << S.G.size() - 1 << "\n";
                }
                else {
                    // 提示输入j，告知范围
                    cout << "Please enter the j index for group " << i_index << " [0, " << S.G[i_index].mu1.size() - 1 << "], or negative for all: ";
                    int j_index;
                    cin >> j_index;
                    cin.ignore();  // 处理换行符

                    if (j_index < 0) {
                        // 输出该组所有j的mu1
                        for (int j = 0; j < S.G[i_index].mu1.size(); j++) {
                            cout << "j = " << j << ": ";
                           
                                cout << S.G[i_index].mu1[j_index] << " ";
                          
                            cout << endl;
                        }
                    }
                    else if (j_index >= S.G[i_index].mu1.size()) {
                        cout << "j index out of range. Valid range: 0 to " << S.G[i_index].mu1.size() - 1 << "\n";
                    }
                    else {
                        // 输出指定i,j的mu1
                        cout << "Values: ";
                        
                            cout << S.G[i_index].mu1[j_index] << " ";
                       
                        cout << endl;
                    }
                }
            }
        }

        
        if (command == "mu2") {
                // 检查S.G是否为空
                if (S.G.empty()) {
                    cout << "S.G is empty. Nothing to display.\n";
                }
                else {
                    // 提示输入i，告知范围
                    cout << "Please enter the i index [0, " << S.G.size() - 1 << "], or negative for all: ";
                    int i_index;
                    cin >> i_index;
                    cin.ignore();  // 处理换行符

                    if (i_index < 0) {
                        // 输出所有组
                        for (int i = 0; i < S.G.size(); i++) {
                            cout << "Group " << i << ":\n";
                            if (S.G[i].Xform.mu2.empty()) {
                                cout << "  (no mu2 data in this group)\n";
                            }
                            else {
                                // 输出该组所有j的mu2
                                for (int j = 0; j < S.G[i].Xform.mu2.size(); j++) {
                                    cout << "  j = " << j << ": ";
                                    for (double val : S.G[i].Xform.mu2) {
                                        cout << val << " ";
                                    }
                                    cout << endl;
                                }
                            }
                        }
                    }
                    else if (i_index >= S.G.size()) {
                        cout << "i index out of range. Valid range: 0 to " << S.G.size() - 1 << "\n";
                    }
                    else {
                        // 提示输入j，告知范围
                        cout << "Please enter the j index for group " << i_index << " [0, " << S.G[index].Xform.mu2.size() - 1 << "], or negative for all: ";
                        int j_index;
                        cin >> j_index;
                        cin.ignore();  // 处理换行符

                        if (j_index < 0) {
                            // 输出该组所有j的mu2
                            for (int j = 0; j < S.G[i_index].Xform.mu2.size(); j++) {
                                cout << "j = " << j << ": ";
                                for (double val : S.G[i_index].Xform.mu2) {
                                    cout << val << " ";
                                }
                                cout << endl;
                            }
                        }
                        else if (j_index >= S.G[i_index].Xform.mu2.size()) {
                            cout << "j index out of range. Valid range: 0 to " << S.G[i_index].Xform.mu2.size() - 1 << "\n";
                        }
                        else {
                            // 输出指定i,j的mu2
                            cout << "Values: ";
                           
                                cout << S.G[i_index].Xform.mu2[j_index] << " ";
                          
                            cout << endl;
                        }
                    }
                }
            }

        // Other parts
        else {
            cout << "Unknown variable: " << command << endl;
        }
    }
}




int main(int argc, char* argv[]) {
    int iflag = 0;
    int method = 0;
    int stepB = 0;  // 新增
    int stepA = 0;  // 新增
    int n = 0;
    double eps = 0.0;
    int order = 0;
    double T = 0;
    int debug = 0;
    std::vector<std::string> SVar;
    std::vector<std::string> SFun;
    IVector B;

    try {
        if (argc < 9) {  // 原来是7，增加了两个参数
            throw std::runtime_error("Insufficient number of arguments provided. Run default example");
        }

        if (argc > 1) {
            iflag = std::stoi(argv[1]);
        }

        if (argc > 2) {
            method = std::stoi(argv[2]);
        }

        if (argc > 3) {
            stepB = std::stoi(argv[3]);  // 新增
        }

        if (argc > 4) {
            stepA = std::stoi(argv[4]);  // 新增
        }

        if (argc > 5) {
            n = std::stoi(argv[5]);
        }

        // check n
        if (n <= 0) {
            throw std::runtime_error("Invalid value for n. n must be a positive integer.");
        }

        for (int i = 0; i < n; ++i) {
            if (argc > 6 + i) {
                SVar.push_back(std::string(argv[6 + i]));
            }
            else {
                throw std::runtime_error("Insufficient number of arguments provided for variables.");
            }
        }

        for (int i = 0; i < n; ++i) {
            if (argc > 6 + n + i) {
                SFun.push_back(std::string(argv[6 + n + i]));
            }
            else {
                throw std::runtime_error("Insufficient number of arguments provided for functions.");
            }
        }

        if (argc > 6 + 2 * n) {
            eps = std::stod(argv[6 + 2 * n]);
        }
        else {
            throw std::runtime_error("Insufficient number of arguments provided for epsilon.");
        }

        if (argc > 7 + 2 * n) {
            order = std::stoi(argv[7 + 2 * n]);
        }
        else {
            throw std::runtime_error("Insufficient number of arguments provided for order.");
        }

        if (argc > 8 + 2 * n) {
            T = std::stod(argv[8 + 2 * n]);
        }
        else {
            throw std::runtime_error("Insufficient number of arguments provided for T.");
        }


        if (argc > 9 + 2 * n) {
            debug = std::stod(argv[9 + 2 * n]);
        }
        else {
            throw std::runtime_error("Insufficient number of arguments provided for debug.");
        }
        // Check enough arguments for intervals
        if (argc < 10 + 3 * n) {
            throw std::runtime_error("Insufficient number of arguments provided for variables, functions, or intervals.");
        }

        // interval
        B.resize(n);
        for (int i = 0; i < n; ++i) {
            double lo = std::stod(argv[10 + 2 * n + 2 * i]);
            double hi = std::stod(argv[10 + 2 * n + 2 * i + 1]);
            B[i] = capd::interval(lo, hi);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << "Reverting to default parameters.\n";

        // default fallback values
        iflag = 0;
        method = 0;
        stepB = 0;
        stepA = 0;
        n = 2;
        eps = 0.1;
        order = 3;
        T = 1;
        debug = 0;
        SVar = { "x", "y" };
        SFun = { "2*x-2*x*y", "-y+x*y" };
        B.resize(n);
        B[0] = capd::interval(0.9, 1.1);
        B[1] = capd::interval(2.9, 3.1);
    }

  

    double veps= eps;
    double  delta =  veps;
    for (int i = 0; i < n; ++i) {
        SFun[i] = expand_expression(SFun[i]);

    }
    std::string sumF = Convert_to_IMap(SVar, SFun);
   int degree = order;

    IMap F1(sumF,3.0);
    IMap F(sumF,3.0);

    
    // B1 = IntervalTaylor(F1, degree, 0.7, B);
    // cout << "B1" << B1 << endl;
    IMatrix JJ = computeJacobian(F, B);
    IMap finp3(Convert_to_IMap(SVar, SVar), 2.0);
    IMap fp3(Convert_to_IMap(SVar, SVar), 2.0);
    // Initialize ReturnType
    ReturnType ret = {
        SVar,   // p2
        SVar,
        finp3,
        fp3,
        {1000.0},                      // mu
        SFun, // ggv
        SVar, // inp2
        -1,                       // d
         SFun,
         F,   // gg
        B,          // B3 (IVector example)
       SVar, // inp3
        B,           // B4 (IVector example)
         JJ
    };
    ministeps G = {
        ret,
        {1000},
        delta,
        T,
        0,
        {B},  // E (IVector example)
        {B}   // F (IVector example)
    };
    Stage S = {
        {0.0},           // T
        {B},  // E (IVector example)
        {B},   // F (IVector example)
        {G}
    };
  
   
   // double veps = 3.3;
   // if (method == 0) { delta = 0.01 * veps; }
    
    double t0 = 0.0;
    double h;

    IVector GE;
    ReturnType2 return1;
    double len1;

  
      //  cout<< "mu" << mu << endl;

    // Recording start time
  

   //  IMap GG2("var:x,y;fun:x^(-0.01),x+y;", 3);
   //  cout << GG(B) << endl;
   //  IMatrix J = computeJacobian(GG, B);
   //  cout << J<< endl;
      // Recording start time
    std::vector<double> timesteprecord = {0};
    auto start1 = std::chrono::high_resolution_clock::now();
    Extendnew(SVar, SFun, F,  S, veps, delta, degree, 1, stepB, stepA,debug);
    cout << S.G[1].Xform.gg << endl;
    //cout << S.E.back() << endl;
    t0 = S.T.back();
    cout << "t0=" << t0 << endl;
    cout << S.G[1].bfE[0] << endl;
    switch (method) {
    case 0:   Refinenew(S, SVar, n, F, stepB, degree, veps,debug); break;
    case 1:  Refinenewsimple(S, SVar, n, F, stepB, degree, veps,debug); break;
    case 2:  RefinenewsimpleT(S, SVar, n, F, stepB, degree, veps,debug); break;
    case 3:   Refinenewnotransform(S, SVar, n, F, stepB, degree, veps,debug); break;
    case 4:   Refinenewnoeuler(S, SVar, n, F, stepB, degree, veps,debug); break;
    }
   
   // cout << S.E.back() << endl;
    double htmp=0;
    while (t0 < T)
    {
        if (t0 + 1 < T) { htmp = 1; }
        else { htmp = T - t0; }
        Extendnew(SVar, SFun, F, S, veps, delta, degree, htmp, stepB, stepA,debug);
        t0 = S.T.back();
        timesteprecord.push_back(t0);

        switch (method) {
        case 0:   Refinenew(S, SVar, n, F, stepB, degree, veps,debug); break;
        case 1:  Refinenewsimple(S, SVar, n, F, stepB, degree, veps,debug); break;
        case 2:  RefinenewsimpleT(S, SVar, n, F, stepB, degree, veps,debug); break;
        case 3:   Refinenewnotransform(S, SVar, n, F, stepB, degree, veps,debug); break;
        case 4:   Refinenewnoeuler(S, SVar, n, F, stepB, degree, veps,debug); break;
        }
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    // Compute run time (in milliseconds)
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);


   // cout << S.E.back() << endl;
    
        auto E = S.E;
       

        std::cout << "Total Time(ms): " << duration1.count() << std::endl;
        if (iflag >=1) {
            std::cout << "initial-enclosure:  center: "  << center(E.front()) << ", length: " << wmax(E.front()) <<" " << "box:" << E.front() << std::endl;

            std::cout << "end-enclosure: center: "<< center(E.back()) << ", length: " << wmax(E.back()) <<" " << "box:" << E.back() << std::endl;
        }
        

        if (iflag >= 2) {
            std::cout << "number of stages: " << timesteprecord.size() << std::endl;
            double num = 0;
            for (int idex = 0; idex < timesteprecord.size(); ++idex) {
                num = num + pow(2, S.G[idex].ell);
            }
            std::cout << "number of boxes in mini-steps: " << num << std::endl;
        }

        if (iflag >= 3) {
            // 输出 Q.T
           // std::cout << "Q.T vector: ";
           // for (const auto& t : Q.T) {
            //    std::cout << t << " ";
          //  }
          //  std::cout << std::endl;

            // 找出 Q.T 中每个时间值在 D.T 中的匹配项，并输出对应的 D.E 值
           // std::cout << "Matching D.E values row-by-row:\n";
            for (size_t i = 0; i < E.size(); ++i) {
                std::cout << "stage" << "[" << i << "]"
					<< "times:" << S.T[i]
					<< "Center:" << center(E[i]) << " length: " << wmax(E[i]) << std::endl;

            }
        }

        if (iflag == 4) {
            interactiveShell(S);
        }

        if (iflag == 8) {
            std::cout << std::setw(10) << "stagetime:" << " | "
                << std::setw(20) << "E:" << " | "
                << std::setw(10) << "mu1:" << " | "
                << std::setw(10) << "mu2:" << " | "
                << std::setw(10) << "Count ministeps" << std::endl;
            std::cout << std::string(75, '-') << std::endl;

            for (size_t i = 0; i < timesteprecord.size(); ++i) {
                // 统计区间内 D.T 的值
                double t_start = timesteprecord[i];
                double t_end = (i + 1 < timesteprecord.size()) ? timesteprecord[i + 1] : t_start;
                int count_in_range = std::count_if(S.T.begin(), S.T.end(), [&](double dt) {
                    return dt >= t_start && dt < t_end;
                    });
                std::cout << std::setw(10) << t_start << " | "
                    << std::setw(20) << "Center:" << center(S.E[i])
                    << " length: " << wmax(S.E[i]) << " | ";
                
               
            }
        }


        if (iflag == 9) {
            std::cout << std::setw(10) << "ministepsT:" << " | "
                << std::setw(20) << "E:" << " | "
                << std::setw(10) << "mu1:" << " | "
                << std::setw(10) << "mu2:" << std::endl;
            std::cout << std::string(60, '-') << std::endl;
            int indexi = 0;
            // 遍历 D.T 并查找是否在 Q.T 中有匹配的值
            for (size_t i = 0; i < S.T.size(); ++i) {
                double dt = S.T[i];
              
                    std::cout << "【" << std::setw(7) << dt << "】 | " // 使用全角字符框
                        << std::setw(20) << "Center:" << center(S.E[i])
                        << " length: " << wmax(S.E[i]) << " | ";
               
               for (size_t q_index = 0; q_index < S.G[i].mu1.size(); ++q_index)
               {
                        std::cout << std::setw(10) << S.G[i].mu1[q_index] << " | "
                            << std::setw(10) << S.G[i].Xform.mu2[q_index] << endl;;
               }
              

               
            }
        }


        cout << "Next: Let eps=wmax(E.back())/2 and call S.refine(eps)" << endl;

        len1 = wmax(E.back());
        auto start2 = std::chrono::high_resolution_clock::now();
       
        Refinenew(S, SVar, n, F, stepB, degree, len1/2,debug);
        auto end2 = std::chrono::high_resolution_clock::now();
        // runtime
        auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);

        std::cout << "Runtime: " << duration2.count() << " ms" << std::endl;
        E = S.E;
        std::cout << "initial-enclosure:  center: " << center(E.front()) << ", length: " << wmax(E.front()) << " " << "box:" << E.front() << std::endl;

        std::cout << "end-enclosure: center: " << center(E.back()) << ", length: " << wmax(E.back()) << " " << "box:" << E.back() << std::endl;
      
        double num = 0;
        for (int idex = 0; idex < timesteprecord.size(); ++idex) {
            num = num + pow(2, S.G[idex].ell);
        }
        std::cout << "number of boxes in mini-steps: " << num << std::endl;
        cout << "Next Let eps=wmax(E.back())/2 and call S.refine(eps)" << endl;
        len1 = wmax(E.back());
        auto start3 = std::chrono::high_resolution_clock::now();
        Refinenew(S, SVar, n, F, stepB, degree, len1 / 2,debug);
        auto end3 = std::chrono::high_resolution_clock::now();
        // runtime 
        auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3);

        std::cout << "Runtime: " << duration3.count() << " ms" << std::endl;
        E = S.E;
        std::cout << "initial-enclosure:  center: " << center(E.front()) << ", length: " << wmax(E.front()) << " " << "box:" << E.front() << std::endl;

        std::cout << "end-enclosure: center: " << center(E.back()) << ", length: " << wmax(E.back()) << " " << "box:" << E.back() << std::endl;

        num = 0;
        for (int idex = 0; idex < timesteprecord.size(); ++idex) {
            num = num + pow(2, S.G[idex].ell);
        }
        std::cout << "number of boxes in mini-steps: " << num << std::endl;
    // Open file for writing LaTeX table



    return 0;
}//main

   
