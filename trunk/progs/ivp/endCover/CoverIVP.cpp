/* file: CoverIVP.cpp
 *
 *      Usage:
 *              > CoverIVP.exe iflag mode method stepB stepA n \
 *                    <var1 ... varn> <fun1 ... funn> eps order T debug \
 *                    <lo1 hi1> ... <lon hin>
 *
 *      This program computes rigorous enclosures (interval boxes) for an
 *      initial value problem (IVP) defined by an ODE system:
 *
 *              x' = f(x),   x(0) ∈ B0
 *
 *      where B0 is an n-dimensional interval box.  The program advances
 *      the enclosure up to final time T using CAPD-based validated integration
 *      and additional refinement / subdivision strategies.
 *
 *      INPUT FORMAT (command line):
 *
 *              iflag   : output level (progressive; larger includes smaller)
 *              mode    : 1 -> boundary-based algorithms (2D/3D)
 *                        0 -> EndCover-based subdivision (cover at time T)
 *              method  : refinement strategy selector (0..6)
 *              stepB   : stepper type for B-space propagation (passed to Extendnew/stepB*)
 *              stepA   : stepper type for auxiliary computations (passed through Extendnew)
 *              n       : system dimension
 *
 *              <var1 ... varn>   : variable names (strings)
 *              <fun1 ... funn>   : RHS expressions f_i (strings)
 *
 *              eps     : target tolerance (veps)
 *              order   : Taylor/Cn degree used by CAPD solvers
 *              T       : final time horizon
 *              debug   : debug level (0/1)
 *
 *              <lo_i hi_i>       : 2*n numbers specifying initial box B0
 *
 *      ALTERNATE INPUT:
 *
 *              If iflag == -1, parameters are read from "input1.txt" with format:
 *                  method stepB stepA n
 *                  <var1 ... varn>
 *                  <fun1 ... funn>
 *                  eps order T debug
 *                  <lo1 hi1> ... <lon hin>
 *
 *      OUTPUT (controlled by iflag):
 *
 *              iflag >= 0:
 *                      prints running time in milliseconds.
 *
 *              iflag >= 1:
 *                      prints the axis-aligned hull (minimal bounding box)
 *                      of all boxes at time T.
 *
 *              iflag >= 2:
 *                      prints number of generated initial sub-boxes (E0Boxes).
 *
 *              iflag >= 3:
 *                      writes:
 *                          "E0.txt" : all initial boxes used for propagation
 *                          "E1.txt" : the resulting cover at time T
 *
 *              iflag >= 4:
 *                      additionally writes plotting-friendly files:
 *                          "E_0.txt" : E0 plus extra corner boxes (2D)
 *                          "E_1.txt" : E1 at time T plus intermediate snapshots
 *                                     at times {0.1, 0.4, 0.7} (if <= T)
 *

 *
 *      IMPORTANT NOTES:
 *
 *              - Expressions in <fun_i> are expanded via expand_expression()
 *                before being converted into a CAPD IMap.
 *
 *              - The refinement method is selected by 'method' and dispatched to:
 *                    Refinenewcp / Refinenewsimple / RefinenewsimpleT /
 *                    Refinenewnotransform / Refinenewnoeuler /
 *                    Refinenewnoeulerplus / (none)
 *
 *              - Boundary algorithms include guards against non-finite or
 *                excessively large interval bounds to prevent downstream failures.
 *
 *      KNOWN ISSUES / TODO:
 *
 *              - Some interactive/debug printing and legacy code blocks remain
 *                (guarded by comments / #if 0) and can be cleaned up.
 *
 *              - Several loops contain manual safeguards to avoid stagnation
 *                when interval endpoints do not advance due to floating precision.
 *
 *              - Buffer sizes and max split depths (e.g., EndCover maxSplits,
 *                TwoDimEncAlgo maxSplitDepth) are fixed constants and may need
 *                tuning for large problems.
 *
 *      REFERENCES:
 *
 *              CAPD library: validated ODE integration and interval arithmetic.
 *
 *      Author: <Bingwei Zhang and Chee Yap>
 *      Date:   <Feb 2026>
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <chrono>
// NOTE: This file is intentionally touched to force rebuilds when headers change.
#include <vector>
#include <string>
#include <iomanip> 
#include <cstdlib> 
#include <functional>
#include <stdexcept>
#include <cmath>
#include "capd/capdlib.h"
#include "Extend-new.h"
#include "EndCover-new.h"

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
    std::string inputLine;
    cout << "Entering interactive mode, input variable names, for example: "<<endl;
    cout << "1:Times list" << endl;
    cout << "2:End-enclosures" << endl;
    cout << "3:Full-enclosures" << endl;
    cout << "4:lognorm list in x-space" << endl;
    cout << "5:lognorm list in y-space" << endl;
    cout << "type '9' to quit." << endl;

    while (true) {
        cout << "> ";
        std::getline(std::cin, inputLine);  // 正确读取整行用户输入

        std::stringstream ss(inputLine);
        int command = -1;
        ss >> command;

        if (command == 9) {
            cout << "Exiting interactive mode.\n";
            break;
        }

        int index = -1;
    
        // Query return1.calD.T
        if (command == 1) {

            cout << "Please enter the index value [0, " << S.T.size() - 1 << "], (input a negative number to view all): ";
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

        if (command == 2) {
            cout << "Please enter the index value [0, " << S.E.size() - 1 << "], (input a negative number to view all): ";
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

        if (command == 3) {
            cout << "Please enter the index value [0, " << S.F.size() - 1 << "], (input a negative number to view all): ";
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
        if (command == 4) {
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
            
        
        if (command == 5) {
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
 
       
    }
}

// 将一个 vector<IVector> 写入文件
static void saveBoxes(const std::vector<IVector>& boxes, const std::string& filename) {
    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "cannot open file: " << filename << std::endl;
        return;
    }  
    fout << std::fixed << std::setprecision(17);

    for (size_t i = 0; i < boxes.size(); i++) {
        const auto& box = boxes[i];
        fout << "Box " << i << ": ";
        for (int d = 0; d < box.dimension(); d++) {   // CAPD Vector 用 dimension()
            auto interval = box[d];                  // 取出一个区间
            fout << "[" << interval.leftBound()
                << ", " << interval.rightBound() << "]";
            if (d + 1 < box.dimension()) fout << " x ";
        }
        fout << "\n";
    }
    fout.close();
    std::cout << "write to -> " << filename << std::endl;
}

static Stage runBoxToHForEndCover(
    const IVector& B,
    const std::vector<std::string>& SVar,
    const std::vector<std::string>& SFun,
    IMap F,
    const ministeps& templateG,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype,
    int debuglevel,
    int dim,
    int method,
    const std::vector<double>& targetPoint)
{
    ministeps G = templateG;
    if (!G.bfE.empty()) G.bfE[0] = B;
    if (!G.bfF.empty()) G.bfF[0] = B;

    Stage S1 = {
        {0.0},  // T
        {B},    // E
        {B},    // F
        {G}
    };

    double T = 0.0;
    double h = H - T;
    while (T < H) {
        if (h > 1.0) h = 1.0;

        Extendnew(SVar, SFun, F, S1, veps, delta, degree, h, stepBtype, stepAtype, debuglevel);

        IVector tmpE = stepBcrlohner(F, degree, S1.T.back(), S1.E[0]);
        S1.E.back() = IntersectB(S1.E.back(), tmpE);

        switch (method) {
        case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
        case 1:   Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
        case 2:   RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
        case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
        case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
        case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint, H); break;
        case 6:   break;
        default:  break;
        }

        T = S1.T.back();
        h = H - T;
    }

    return S1;
}


void TwoDimEncAlgo(StageBound& Sdata, std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    Stage& S,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype,
    int debuglevel,
    int dim,
    int method,
    int mid)
{
    auto ensureFiniteBoxOrThrow = [](const IVector& box, const char* where) {
        for (int d = 0; d < box.dimension(); ++d) {
            const double L = box[d].leftBound();
            const double R = box[d].rightBound();
            if (!(std::isfinite(L) && std::isfinite(R))) {
                throw std::runtime_error(std::string("non-finite bounds in ") + where);
            }
        }
    };

    auto ensureReasonableMagnitudeOrThrow = [](const IVector& box, const char* where) {
        // Heuristic guard: many downstream routines (e.g. convex hull helpers) may square
        // coordinates; keep bounds safely below ~sqrt(DBL_MAX) ~= 1e154.
        constexpr double maxAbsBound = 1e150;
        for (int d = 0; d < box.dimension(); ++d) {
            const double L = box[d].leftBound();
            const double R = box[d].rightBound();
            if (std::fabs(L) > maxAbsBound || std::fabs(R) > maxAbsBound) {
                throw std::runtime_error(std::string("bounds too large in ") + where);
            }
        }
    };

    auto runBoundaryBoxToH = [&](const IVector& boundaryBox, const std::vector<double>& targetPoint) -> Stage {
        ministeps G = S.G[0];
        G.bfE[0] = boundaryBox;
        G.bfF[0] = boundaryBox;
        Stage S1 = {
            {0.0},         // T
            {boundaryBox}, // E
            {boundaryBox}, // F
            {G}
        };

        double T = 0.0;
        double h = H - T;
        while (T < H) {
            if (h > 1.0) {
                h = 1.0;
            }

            Extendnew(SVar, SFun, F, S1, veps, delta, degree, h, stepBtype, stepAtype, debuglevel);

            IVector tmpE = stepBcrlohner(F, degree, S1.T.back(), S1.E[0]);
            S1.E.back() = IntersectB(S1.E.back(), tmpE);

            // If the enclosure explodes to Inf/NaN, treat it like a failure
            // so the caller can split the boundary segment.
            ensureFiniteBoxOrThrow(S1.E.back(), "TwoDimEncAlgo/afterIntersect");
            ensureReasonableMagnitudeOrThrow(S1.E.back(), "TwoDimEncAlgo/afterIntersect");

            switch (method) {
            case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
            case 1:   Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
            case 2:   RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
            case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
            case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint); break;
            case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, targetPoint, H); break;
            case 6:   break;
            default:  break;
            }

            ensureFiniteBoxOrThrow(S1.E.back(), "TwoDimEncAlgo/afterRefine");
            ensureReasonableMagnitudeOrThrow(S1.E.back(), "TwoDimEncAlgo/afterRefine");

            T = S1.T.back();
            h = H - T;
        }

        return S1;
    };

    auto appendStageResult = [&](const Stage& stage) {
        Sdata.E0.push_back(stage.E[0]);
        if (mid == 1) {
            for (const auto& x : stage.E) {
                Sdata.E1.push_back(x);
            }
        }
        else {
            Sdata.E1.push_back(stage.E.back());
        }
    };

    IVector Sbound(dim);
    bool hasSbound = false;

    // Splitting parameters: we prefer "never drop" over "always finish".
    // If a segment still cannot be processed even after very deep splitting,
    // we throw with a clear message rather than silently skipping it.
    const int maxSplitDepth = 60;
    const double minSplitWidth = 0.0;

    std::function<void(int /*fixedDim*/, interval /*fixedValue*/, int /*varDim*/, double /*a*/, double /*b*/, std::function<std::vector<double>(const IVector&)>, int /*depth*/)> processEdge;
    processEdge = [&](int fixedDim, interval fixedValue, int varDim, double a, double b, std::function<std::vector<double>(const IVector&)> makeTarget, int depth) {
        if (!(a < b)) {
            return;
        }

        double current = a;
        while (current < b) {
            IVector edgeBox = S.E[0];
            edgeBox[fixedDim] = fixedValue;
            edgeBox[varDim] = interval(current, b);

            try {
                Stage stage = runBoundaryBoxToH(edgeBox, makeTarget(edgeBox));
                appendStageResult(stage);

                if (!hasSbound) {
                    Sbound = stage.E.back();
                    hasSbound = true;
                }
                else {
                    Sbound = Box(Sbound, stage.E.back());
                }

                double next = stage.E[0][varDim].rightBound();
                if (!(next > current)) {
                    // 强制推进，避免死循环
                    next = (current + b) / 2.0;
                }
                current = next;
            }
            catch (const std::exception& e) {
                const double width = b - current;
                if (width <= minSplitWidth || depth >= maxSplitDepth) {
                    // Last resort: attempt point processing at `current`.
                    IVector pointBox = S.E[0];
                    pointBox[fixedDim] = fixedValue;
                    pointBox[varDim] = interval(current, current);
                    try {
                        Stage stage = runBoundaryBoxToH(pointBox, makeTarget(pointBox));
                        appendStageResult(stage);
                        if (!hasSbound) {
                            Sbound = stage.E.back();
                            hasSbound = true;
                        }
                        else {
                            Sbound = Box(Sbound, stage.E.back());
                        }
                        current = b;
                        continue;
                    }
                    catch (const std::exception& e2) {
                        throw std::runtime_error(
                            std::string("TwoDimEncAlgo failed to process edge segment even after splitting: [") +
                            std::to_string(current) + ", " + std::to_string(b) + "] (depth=" +
                            std::to_string(depth) + "). Last error: " + e2.what());
                    }
                }

                double midPoint = (current + b) / 2.0;
                if (!(midPoint > current && midPoint < b)) {
                    // Ensure progress even when (current+b)/2 rounds to an endpoint.
                    midPoint = std::nextafter(current, b);
                    if (!(midPoint > current && midPoint < b)) {
                        throw std::runtime_error(
                            std::string("TwoDimEncAlgo cannot split further due to floating precision on segment [") +
                            std::to_string(current) + ", " + std::to_string(b) + "]");
                    }
                }

                processEdge(fixedDim, fixedValue, varDim, current, midPoint, makeTarget, depth + 1);
                processEdge(fixedDim, fixedValue, varDim, midPoint, b, makeTarget, depth + 1);
                return;
            }
            catch (...) {
                const double width = b - current;
                if (width <= minSplitWidth || depth >= maxSplitDepth) {
                    throw std::runtime_error(
                        std::string("TwoDimEncAlgo failed to process edge segment even after splitting: [") +
                        std::to_string(current) + ", " + std::to_string(b) + "] (depth=" +
                        std::to_string(depth) + ") due to unknown error");
                }

                double midPoint = (current + b) / 2.0;
                if (!(midPoint > current && midPoint < b)) {
                    midPoint = std::nextafter(current, b);
                    if (!(midPoint > current && midPoint < b)) {
                        throw std::runtime_error(
                            std::string("TwoDimEncAlgo cannot split further due to floating precision on segment [") +
                            std::to_string(current) + ", " + std::to_string(b) + "]");
                    }
                }
                processEdge(fixedDim, fixedValue, varDim, current, midPoint, makeTarget, depth + 1);
                processEdge(fixedDim, fixedValue, varDim, midPoint, b, makeTarget, depth + 1);
                return;
            }
        }
    };

    // Left edge: x fixed at left bound, y varies
    {
        const double xL = S.E[0][0].leftBound();
        const double yA = S.E[0][1].leftBound();
        const double yB = S.E[0][1].rightBound();
        processEdge(0, interval(xL, xL), 1, yA, yB,
            [&](const IVector& box) {
                return std::vector<double>{ box[0].leftBound(), box[1].leftBound() };
            },
            0);
        if (debuglevel == 1) cout << "1" << endl;
    }

    // Right edge: x fixed at right bound, y varies
    {
        const double xR = S.E[0][0].rightBound();
        const double yA = S.E[0][1].leftBound();
        const double yB = S.E[0][1].rightBound();
        processEdge(0, interval(xR, xR), 1, yA, yB,
            [&](const IVector& box) {
                return std::vector<double>{ box[0].rightBound(), box[1].leftBound() };
            },
            0);
        if (debuglevel == 1) cout << "2" << endl;
    }

    // Up edge: y fixed at upper bound, x varies
    {
        const double yU = S.E[0][1].rightBound();
        const double xA = S.E[0][0].leftBound();
        const double xB = S.E[0][0].rightBound();
        processEdge(1, interval(yU, yU), 0, xA, xB,
            [&](const IVector& box) {
                return std::vector<double>{ box[0].leftBound(), box[1].rightBound() };
            },
            0);
        if (debuglevel == 1) cout << "3" << endl;
    }

    // Bottom edge: y fixed at lower bound, x varies
    {
        const double yBtm = S.E[0][1].leftBound();
        const double xA = S.E[0][0].leftBound();
        const double xB = S.E[0][0].rightBound();
        processEdge(1, interval(yBtm, yBtm), 0, xA, xB,
            [&](const IVector& box) {
                return std::vector<double>{ box[0].leftBound(), box[1].leftBound() };
            },
            0);
    }

    if (hasSbound) {
        Sdata.E = Sbound;
    }
    else {
        Sdata.E = S.E[0];
    }

}



void TwoDimEncAlgounfinish(StageBound& Sdata, std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    Stage& S,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype,
    int debuglevel,
    int dim,
    int method)
{
    IVector Sbound(dim);

    // 初始化四个边界盒子
    IVector LeftB(dim);
    LeftB[0] = interval(S.E[0][0].leftBound(), S.E[0][0].leftBound());
    LeftB[1] = S.E[0][1];
   //cout << "leftB=" << LeftB << endl;
    IVector RightB(dim);
    RightB[0] = interval(S.E[0][0].rightBound(), S.E[0][0].rightBound());
    RightB[1] = S.E[0][1];

    IVector UpB(dim);
    UpB[1] = interval(S.E[0][1].rightBound(), S.E[0][1].rightBound());
    UpB[0] = S.E[0][0];

    IVector BottomB(dim);
    BottomB[1] = interval(S.E[0][1].leftBound(), S.E[0][1].leftBound());
    BottomB[0] = S.E[0][0];

    ministeps G = S.G[0];
    G.bfE[0] = LeftB;
    G.bfF[0] = LeftB;
    double T1 = 0;
    std::vector<double> target = { LeftB[0].leftBound(), LeftB[1].leftBound() };

    Stage S1 = {
        {0.0},    // T
        {LeftB},  // E
        {LeftB},  // F
        {G}
    };
    
    double h1 = H-T1;
    ICnOdeSolver solverL(F, degree);
    solverL.setAbsoluteTolerance(1e-10);
    solverL.setRelativeTolerance(1e-10);
   
    // === 初始化状态集合 ===
    CnMultiMatrixRect2Set setL(S1.E[0], 3.0); // 初始包络
   
    while (T1 < H) {
        if (h1 > 1.0) { h1 = 1.0; }

        Extendnew(SVar, SFun, F, S1, veps, delta, degree,h1 , stepBtype, stepAtype, debuglevel);
      //  IVector TMPE = S1.E[S1.E.size() - 1];
       // int numberOfSteps = 10;
        double TotalH = 0;
        double tmph = S1.T.back() - S1.T[S1.T.size() - 2];
      //  cout << "tmph=" << tmph << endl;
        //double timeStep = tmph / numberOfSteps;

       // solverL.setStep(timeStep);
       
        while (TotalH < tmph) {
           
            double minih = solverL.getStep().leftBound();
            if (minih < 0.01) {
                minih = 0.01;
            }
            if (TotalH+ minih > tmph) {
                solverL.setStep(tmph - TotalH);
                TotalH = tmph;
                setL.move(solverL);
            }
            else {
                solverL.setStep(minih);
                setL.move(solverL);
                TotalH = TotalH + minih;
            }
        }
       
        cout << "S1.E.back()=" << S1.E.back() << endl;
       cout << "Set" << IVector(setL) << endl;
        S1.E.back() = IntersectB(S1.E.back(), IVector(setL));
        switch (method) {
        case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
        case 1:  Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
        case 2:  RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
        case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
        case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
        case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target,H); break;
        } T1 = S1.T.back();
        h1 = H - T1;
        
    }
    cout << "0" << endl;
   // cout << "E0=" << S1.E[0] << endl;
    Sdata.E0.push_back(S1.E[0]);
   
  
    Sdata.E1.push_back(S1.E.back());
    LeftB[1] = interval(S1.E[0][1].rightBound(), S.E[0][1].rightBound());
    Sbound = S1.E.back();
   // cout << "edfinB=" << S1.E.back() << endl;
    // 处理 Left 边界
    while (LeftB[1].leftBound() < LeftB[1].rightBound()) {
      //  cout << "leftB="<<LeftB[1].leftBound() << endl;
        double T = 0; double h = H -T;
        std::vector<double> target = { LeftB[0].leftBound(), LeftB[1].leftBound() };
        G.bfE[0] = LeftB;
        G.bfF[0] = LeftB;
        Stage S1 = {
            {0.0},    // T
            {LeftB},  // E
            {LeftB},  // F
            {G}
        };
        ICnOdeSolver solverL2(F, degree);
        solverL2.setAbsoluteTolerance(1e-10);
        solverL2.setRelativeTolerance(1e-10);
       
        // === 初始化状态集合 ===
        CnMultiMatrixRect2Set setL2(S1.E[0], 3.0); // 初始包络

        while (T < H) {
            if (h > 1.0) { h = 1.0; }
            Extendnew(SVar, SFun, F, S1, veps, delta, degree, h, stepBtype, stepAtype, debuglevel);
            
            
            double tmph = S1.T.back() - S1.T[S1.T.size() - 2];
            //  cout << "tmph=" << tmph << endl;
              //double timeStep = tmph / numberOfSteps;

             // solverL.setStep(timeStep);
            double minih = solverL2.getStep().leftBound();
            cout << "minh=" << minih << endl;
            if (minih < 0.01) {
                minih = 0.01;
            }
            if (minih > tmph) {
                minih = tmph;
            }
            while (solverL2.getCurrentTime().rightBound() < S1.T.back()) {
                

                if (solverL2.getCurrentTime().rightBound() + minih > S1.T.back()) {
                    solverL2.setStep(S1.T.back() - solverL2.getCurrentTime().rightBound());
                    
                    setL2.move(solverL2);
                }
                else {
                    solverL2.setStep(minih);
                    setL2.move(solverL2);
                  
                }
            }

              cout << "S1.E.back()=" << S1.E.back()<<S1.T.back() << endl;
              cout << "Set" << IVector(setL) <<solverL2.getCurrentTime()<< endl;
            S1.E.back() = IntersectB(S1.E.back(), IVector(setL2));



            switch (method) {
            case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 1:  Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 2:  RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target,H); break;
            }
            T = S1.T.back(); h = H - T;
        }  

        Sdata.E0.push_back(S1.E[0]); 


        Sdata.E1.push_back(S1.E.back());
       // cout << "E0=" << S1.E[0] << endl;
        LeftB[1] = interval(S1.E[0][1].rightBound(), S.E[0][1].rightBound());
       // cout << "leftB=" << LeftB << endl;
       // cout << "edfinB=" << S1.E.back() << endl;
        Sbound = Box(Sbound, S1.E.back());
    }
    if (debuglevel == 1) cout << "1" << endl;
    // 处理 Right 边界
    while (RightB[1].leftBound() < RightB[1].rightBound()) {
        double T = 0; double h = H - T;
        std::vector<double> target = { RightB[0].rightBound(), RightB[1].leftBound() };
        G.bfE[0] = RightB;
        G.bfF[0] = RightB;
        Stage S1 = {
            {0.0},    // T
            {RightB}, // E
            {RightB}, // F
            {G}
        };
        ICnOdeSolver solverR(F, degree);
        solverR.setAbsoluteTolerance(1e-10);
        solverR.setRelativeTolerance(1e-10);

        // === 初始化状态集合 ===
        CnMultiMatrixRect2Set setR(S1.E[0], 3.0); // 初始包络
        while (T < H) {
            if (h > 1.0) { h = 1.0; }
            Extendnew(SVar, SFun, F, S1, veps, delta, degree, h, stepBtype, stepAtype, debuglevel);
           

            double TotalH = 0;
            double tmph = S1.T.back() - S1.T[S1.T.size() - 2];
            //  cout << "tmph=" << tmph << endl;
              //double timeStep = tmph / numberOfSteps;

             // solverL.setStep(timeStep);

            while (TotalH < tmph) {
                double minih = solverR.getStep().leftBound();
                if (minih < 0.01) {
                    minih = 0.01;
                }

                if (TotalH + minih > tmph) {
                    solverR.setStep(tmph - TotalH);
                    TotalH = tmph;
                    setR.move(solverR);
                }
                else {
                    solverR.setStep(minih);
                    setR.move(solverR);
                    TotalH = TotalH + minih;
                }
            }

            //  cout << "S1.E.back()=" << S1.E.back() << endl;
            //  cout << "Set" << IVector(setL) << endl;
            S1.E.back() = IntersectB(S1.E.back(), IVector(setR));

            switch (method) {
            case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 1:  Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 2:  RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target,H); break;
            }
            T = S1.T.back();  h = H - T;
        }
        Sdata.E0.push_back(S1.E[0]);


        Sdata.E1.push_back(S1.E.back());
       // cout << "E0=" << S1.E[0] << endl;
        RightB[1] = interval(S1.E[0][1].rightBound(), S.E[0][1].rightBound());
       // cout << "edfinB=" << S1.E.back() << endl;
        Sbound = Box(Sbound, S1.E.back());
    }
    cout << "2" << endl;
    // 处理 Up 边界
    while (UpB[0].leftBound() < UpB[0].rightBound()) {
        double T = 0; double h = H - T;
        std::vector<double> target = { UpB[0].leftBound(), UpB[1].rightBound() };
        G.bfE[0] = UpB;
        G.bfF[0] = UpB;
        Stage S1 = {
            {0.0},   // T
            {UpB},   // E
            {UpB},   // F
            {G}
        };
        ICnOdeSolver solverU(F, degree);
        solverU.setAbsoluteTolerance(1e-10);
        solverU.setRelativeTolerance(1e-10);

        // === 初始化状态集合 ===
        CnMultiMatrixRect2Set setU(S1.E[0], 3.0); // 初始包络
        while (T < H) {
            if (h > 1.0) { h = 1.0; }
            Extendnew(SVar, SFun, F, S1, veps, delta, degree, h, stepBtype, stepAtype, debuglevel);
          
            double TotalH = 0;
            double tmph = S1.T.back() - S1.T[S1.T.size() - 2];
            //  cout << "tmph=" << tmph << endl;
              //double timeStep = tmph / numberOfSteps;

             // solverL.setStep(timeStep);

            while (TotalH < tmph) {
                double minih = solverU.getStep().leftBound();
                if (minih < 0.01) {
                    minih = 0.01;
                }

                if (TotalH + minih > tmph) {
                    solverU.setStep(tmph - TotalH);
                    TotalH = tmph;
                    setU.move(solverU);
                }
                else {
                    solverU.setStep(minih);
                    setU.move(solverU);
                    TotalH = TotalH + minih;
                }
            }

            //  cout << "S1.E.back()=" << S1.E.back() << endl;
            //  cout << "Set" << IVector(setL) << endl;
            S1.E.back() = IntersectB(S1.E.back(), IVector(setU));

            switch (method) {
            case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 1:  Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 2:  RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target,H); break;
            }  T = S1.T.back(); h = H - T;
        } 
        Sdata.E0.push_back(S1.E[0]);


        Sdata.E1.push_back(S1.E.back());
      //  cout << "E0=" << S1.E[0] << endl;
        UpB[0] = interval(S1.E[0][0].rightBound(), S.E[0][0].rightBound());
       // cout << "edfinB=" << S1.E.back() << endl;
        Sbound = Box(Sbound, S1.E.back());
    }
    cout << "3" << endl;
    // 处理 Bottom 边界
    while (BottomB[0].leftBound() < BottomB[0].rightBound()) {
        double T = 0; double h = H - T;
        std::vector<double> target = { BottomB[0].leftBound(), BottomB[1].leftBound() };
        G.bfE[0] = BottomB;
        G.bfF[0] = BottomB;
        Stage S1 = {
            {0.0},      // T
            {BottomB},  // E
            {BottomB},  // F
            {G}
        };
        ICnOdeSolver solverB(F, degree);
        solverB.setAbsoluteTolerance(1e-10);
        solverB.setRelativeTolerance(1e-10);

        // === 初始化状态集合 ===
        CnMultiMatrixRect2Set setB(S1.E[0], 3.0); // 初始包络
        while (T < H) {
            if (h > 1.0) { h = 1.0; }
            Extendnew(SVar, SFun, F, S1, veps, delta, degree, h, stepBtype, stepAtype, debuglevel);
           

            double TotalH = 0;
            double tmph = S1.T.back() - S1.T[S1.T.size() - 2];
            //  cout << "tmph=" << tmph << endl;
              //double timeStep = tmph / numberOfSteps;

             // solverL.setStep(timeStep);

            while (TotalH < tmph) {

                double minih = solverB.getStep().leftBound();
                if (minih < 0.01) {
                    minih = 0.01;
                }
                if (TotalH + minih > tmph) {
                    solverB.setStep(tmph - TotalH);
                    TotalH = tmph;
                    setB.move(solverB);
                }
                else {
                    solverB.setStep(minih);
                    setB.move(solverB);
                    TotalH = TotalH + minih;
                }
            }

            //  cout << "S1.E.back()=" << S1.E.back() << endl;
            //  cout << "Set" << IVector(setL) << endl;
            S1.E.back() = IntersectB(S1.E.back(), IVector(setB));

            switch (method) {
            case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 1:  Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 2:  RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target); break;
            case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, veps, debuglevel, target,H); break;
            } T = S1.T.back();  h = H - T;
        }
        Sdata.E0.push_back(S1.E[0]);


        Sdata.E1.push_back(S1.E.back());
       // cout << "E0=" << S1.E[0] << endl;
        BottomB[0] = interval(S1.E[0][0].rightBound(), S.E[0][0].rightBound());
       // cout << "edfinB=" << S1.E.back() << endl;
        Sbound = Box(Sbound, S1.E.back());
    }
    Sdata.E = Sbound;
    
}


#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>

// 点结构体
struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // 用于排序和比较
    bool operator<(const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    bool operator==(const Point& p) const {
        return std::abs(x - p.x) < 1e-10 && std::abs(y - p.y) < 1e-10;
    }
};

// 计算叉积 (p1-p0) × (p2-p0)
double cross(const Point& p0, const Point& p1, const Point& p2) {
    return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
}

// 计算两点间距离的平方
double distSq(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

// Andrew's monotone chain 凸包算法
std::vector<Point> convexHull(std::vector<Point>& points) {
    int n = points.size();
    if (n <= 1) return points;

    // 排序并去重
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());
    n = points.size();

    std::vector<Point> hull(2 * n);
    int k = 0;
     
    // 构建下凸包
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    // 构建上凸包
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    hull.resize(k - 1);
    return hull;
}

// 从IVector盒子集合中提取所有顶点
std::vector<Point> extractVertices(const std::vector<IVector>& boxes) {
    std::vector<Point> vertices;

    for (const auto& box : boxes) {
        if (box.dimension() < 2) continue;

        double x0 = box[0].leftBound();
        double x1 = box[0].rightBound();
        double y0 = box[1].leftBound();
        double y1 = box[1].rightBound();

        // 添加四个顶点
        vertices.push_back(Point(x0, y0)); // 左下
        vertices.push_back(Point(x0, y1)); // 左上
        vertices.push_back(Point(x1, y0)); // 右下
        vertices.push_back(Point(x1, y1)); // 右上
    }

    return vertices;
}

// 主函数：计算盒子集合的凸包并保存到文件
void BoxConvexHull(const std::vector<IVector>& E0, const std::string& filename) {
    // 1. 提取所有顶点
    std::vector<Point> vertices = extractVertices(E0);

    if (vertices.empty()) {
        std::cout << "Warning: No vertices found in input boxes." << std::endl;
        return;
    }

    std::cout << "Extracted " << vertices.size() << " vertices from "
        << E0.size() << " boxes." << std::endl;

    // 2. 计算凸包
    std::vector<Point> hull = convexHull(vertices);

    std::cout << "Computed convex hull with " << hull.size() << " vertices." << std::endl;

    // 3. 保存到文件
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing." << std::endl;
        return;
    }

    // 写入文件头
    outFile << "# Convex Hull of Box Collection" << std::endl;
    outFile << "# Number of input boxes: " << E0.size() << std::endl;
    outFile << "# Number of hull vertices: " << hull.size() << std::endl;
    outFile << "# Format: x y" << std::endl;

    // 写入凸包顶点（按顺序）
    for (const auto& point : hull) {
        outFile << point.x << " " << point.y << std::endl;
    }

    // 为了形成闭合多边形，再次写入第一个点
    if (!hull.empty()) {
        outFile << hull[0].x << " " << hull[0].y << std::endl;
    }

    outFile.close();
    std::cout << "Convex hull saved to " << filename << std::endl;

    // 可选：在控制台输出凸包信息
    std::cout << "Convex hull vertices:" << std::endl;
    for (const auto& point : hull) {
        std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    }
}

// 辅助函数：验证凸包是否正确包含所有盒子
void validateConvexHull(const std::vector<IVector>& boxes, const std::vector<Point>& hull) {
    // 简单的验证：检查所有盒子的顶点是否都在凸包内或边界上
    // 这里使用射线法进行点是否在多边形内的测试

    auto pointInPolygon = [](const Point& p, const std::vector<Point>& poly) -> bool {
        int n = poly.size();
        bool inside = false;

        for (int i = 0, j = n - 1; i < n; j = i++) {
            if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
                (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)) {
                inside = !inside;
            }
        }
        return inside;
        };

    // 测试所有盒子的顶点
    std::vector<Point> testPoints = extractVertices(boxes);
    int insideCount = 0;

    for (const auto& point : testPoints) {
        if (pointInPolygon(point, hull)) {
            insideCount++;
        }
    }

    std::cout << "Validation: " << insideCount << "/" << testPoints.size()
        << " box vertices are inside the convex hull." << std::endl;
}

// 重载版本：包含验证
void BoxConvexHullWithValidation(const std::vector<IVector>& E0, const std::string& filename) {
    // 提取顶点并计算凸包
    std::vector<Point> vertices = extractVertices(E0);
    std::vector<Point> hull = convexHull(vertices);

    // 验证
    validateConvexHull(E0, hull);

    // 保存到文件
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing." << std::endl;
        return;
    }

    outFile << "# Convex Hull with Validation" << std::endl;
    outFile << "# Input boxes: " << E0.size() << std::endl;
    outFile << "# Hull vertices: " << hull.size() << std::endl;
    outFile << "# Format: x y" << std::endl;

    for (const auto& point : hull) {
        outFile << point.x << " " << point.y << std::endl;
    }

    if (!hull.empty()) {
        outFile << hull[0].x << " " << hull[0].y << std::endl;
    }

    outFile.close();
    std::cout << "Convex hull saved to " << filename << std::endl;
}




// 辅助函数：处理单个面
void ProcessFace( std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    Stage& S,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype,
    int debuglevel,
    int dim,
    int method,
    vector<IVector>& E0,
    vector<IVector>& E1)
{  
    std::vector<Stage> target = { S };
    while (!target.empty())
    {
        Stage cur = target.front();
       //cout << target.size() << endl;
        target.erase(target.begin());
      // cout << target.size() << endl;
        IVector tmpE = cur.E[0];
       // cout << "tmpE=" << tmpE << endl;
        std::vector<ministeps> G = cur.G;
        cur = {
            {0,1},
            {tmpE,tmpE},
            {tmpE,tmpE},
            G
        };
        double r0 = veps*2;
        int couter = 0;
        //  cout << "mmu=" << mmu << endl;
        while (veps < r0) {


           
            IVector E0= cur.E[0];
            if (couter > 0) {
                // 获取盒子的维数

                int n = dim;
                IVector OrignE = cur.E[0];
                // 计算每个维度需要分割的中点
                std::vector<double> midpoints(n);
                for (int i = 0; i < n; ++i) {
                    midpoints[i] = (OrignE[i].leftBound() + OrignE[i].rightBound()) / 2.0;
                }

                // 保留的盒子E0（我们选择mask=0对应的盒子作为保留的盒子）
                IVector E0(n);
                for (int dim1 = 0; dim1 < n; ++dim1) {
                    E0[dim1] = capd::interval(OrignE[dim1].leftBound(), midpoints[dim1]);
                }

                // 生成所有可能的子盒子索引组合（2^n个），跳过第一个

                for (int mask = 1; mask < (1 << n); ++mask) {  // 从1开始，跳过0
                    // 创建新的盒子

                    IVector newE(n);

                    // 根据mask的每一位决定每个维度取左半还是右半
                    for (int dim1 = 0; dim1 < n; ++dim1) {
                        if (mask & (1 << dim1)) {
                            // 该位为1，取右半部分 [mid, right]
                            newE[dim1] = capd::interval(midpoints[dim1], OrignE[dim1].rightBound());
                        }
                        else {
                            // 该位为0，取左半部分 [left, mid]
                            newE[dim1] = capd::interval(OrignE[dim1].leftBound(), midpoints[dim1]);
                        }
                    }

                    cur.E[0] = newE;
                    cur.F[0] = newE;
                    cur.G[0].bfE[0] = newE;
                    cur.G[0].bfF[0] = newE;


                    target.push_back(cur);
                }

                cur.E[0] = E0;
                cur.F[0] = E0;
                cur.G[0].bfE[0] = E0;
                cur.G[0].bfF[0] = E0;
            }
          //  cout << "here1" <<E0<< endl;
            cur.E[1] = stepBcrlohner(F, degree, H, E0);
           // cout << "here2" << endl;
            r0 = wmax(cur.E[1]);
            
            couter = couter + 1;
        }



        E0.push_back(cur.E[0]);
        E1.push_back(cur.E.back());
    }
      
} 





#if 0
// Original version (kept for reference): uses ProcessFace-based subdivision.
void ThreeDimEncAlgo(StageBound& Sdata, std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    Stage S,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype,
    int debuglevel,
    int dim,
    int method)
{
    Stage TS = S;
    vector<IVector> E0 = {};
        vector<IVector> E1 = {};
    // 初始化六个面的盒子
    // 1. Bottom face: z = z0
    IVector BottomFace(dim);
    BottomFace[0] = S.E[0][0];
    BottomFace[1] = S.E[0][1];
    BottomFace[2] = interval(S.E[0][2].leftBound(), S.E[0][2].leftBound());

    // 2. Top face: z = z1
    IVector TopFace(dim);
    TopFace[0] = S.E[0][0];
    TopFace[1] = S.E[0][1];
    TopFace[2] = interval(S.E[0][2].rightBound(), S.E[0][2].rightBound());

    // 3. Front face: y = y0
    IVector FrontFace(dim);
    FrontFace[0] = S.E[0][0];
    FrontFace[1] = interval(S.E[0][1].leftBound(), S.E[0][1].leftBound());
    FrontFace[2] = S.E[0][2];

    // 4. Back face: y = y1
    IVector BackFace(dim);
    BackFace[0] = S.E[0][0];
    BackFace[1] = interval(S.E[0][1].rightBound(), S.E[0][1].rightBound());
    BackFace[2] = S.E[0][2];

    // 5. Left face: x = x0
    IVector LeftFace(dim);
    LeftFace[0] = interval(S.E[0][0].leftBound(), S.E[0][0].leftBound());
    LeftFace[1] = S.E[0][1];
    LeftFace[2] = S.E[0][2];

    // 6. Right face: x = x1
    IVector RightFace(dim);
    RightFace[0] = interval(S.E[0][0].rightBound(), S.E[0][0].rightBound());
    RightFace[1] = S.E[0][1];
    RightFace[2] = S.E[0][2];

   

    // 处理 Bottom Face (z = z0)
    cout << "Processing Bottom Face" << endl;
    TS.E[0] = BottomFace;
    TS.F[0] = BottomFace;
    TS.G[0].bfE[0] = BottomFace;
    TS.G[0].bfF[0] = BottomFace;
   
    ProcessFace( SVar, SFun, F, TS, veps, delta, degree, H,
        stepBtype, stepAtype, debuglevel, dim, method, E0,E1);
    TS = S;
    // 处理 Top Face (z = z1)
    cout << "Processing Top Face" << endl;
    TS.E[0] = TopFace;
    TS.F[0] = TopFace;
    TS.G[0].bfE[0] = TopFace;
    TS.G[0].bfF[0] = TopFace;

    ProcessFace( SVar, SFun, F, TS,  veps, delta, degree, H,
        stepBtype, stepAtype, debuglevel, dim, method, E0, E1);
    TS = S;
    // 处理 Front Face (y = y0)
    cout << "Processing Front Face" << endl;
    TS.E[0] = FrontFace;
    TS.F[0] = FrontFace;
    TS.G[0].bfE[0] = FrontFace;
    TS.G[0].bfF[0] = FrontFace;
    ProcessFace( SVar, SFun, F, TS, veps, delta, degree, H,
        stepBtype, stepAtype, debuglevel, dim, method, E0, E1);
    TS = S;
    // 处理 Back Face (y = y1)
    cout << "Processing Back Face" << endl;
    TS.E[0] = BackFace;
    TS.F[0] = BackFace;
    TS.G[0].bfE[0] = BackFace;
    TS.G[0].bfF[0] = BackFace;
    ProcessFace( SVar, SFun, F, TS, veps, delta, degree, H,
        stepBtype, stepAtype, debuglevel, dim, method, E0, E1);
    TS = S;
    // 处理 Left Face (x = x0)
    cout << "Processing Left Face" << endl;
    TS.E[0] = LeftFace;
    TS.F[0] = LeftFace;
    TS.G[0].bfE[0] = LeftFace;
    TS.G[0].bfF[0] = LeftFace;
    ProcessFace( SVar, SFun, F, TS,  veps, delta, degree, H,
        stepBtype, stepAtype, debuglevel, dim, method, E0, E1);
    TS = S;
    // 处理 Right Face (x = x1)
    cout << "Processing Right Face" << endl;
    TS.E[0] = RightFace;
    TS.F[0] = RightFace;
    TS.G[0].bfE[0] = RightFace;
    TS.G[0].bfF[0] = RightFace;
 
    ProcessFace(SVar, SFun, F, TS, veps, delta, degree, H,
        stepBtype, stepAtype, debuglevel, dim, method, E0, E1);

    Sdata = {
        E0,
        E1,
        BottomFace
    };
}
#endif

// New version: process each face using EndCover (subdivision driven by ulB != B)
void ThreeDimEncAlgo(StageBound& Sdata, std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    Stage S,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype,
    int debuglevel,
    int dim,
    int method)
{
    std::vector<IVector> E0;
    std::vector<IVector> E1;
    E0.reserve(1024);
    E1.reserve(1024);

    IVector Sbound(dim);
    bool hasSbound = false;

    auto endEnc = [&](const IVector& B, double epsLocal, const std::vector<double>& p, double Hlocal)
        -> std::pair<IVector, IVector> {
        ministeps G = S.G[0];
        if (!G.bfE.empty()) G.bfE[0] = B;
        if (!G.bfF.empty()) G.bfF[0] = B;

        Stage S1 = {
            {0.0},
            {B},
            {B},
            {G}
        };

        double Tcur = 0.0;
        double h = Hlocal - Tcur;
        while (Tcur < Hlocal) {
            if (h > 1.0) {
                h = 1.0;
            }

            Extendnew(SVar, SFun, F, S1, epsLocal, delta, degree, h, stepBtype, stepAtype, debuglevel);

            // Keep behavior consistent with other boundary runners.
            IVector tmpE = stepBcrlohner(F, degree, S1.T.back(), S1.E[0]);
            S1.E.back() = IntersectB(S1.E.back(), tmpE);

            switch (method) {
            case 0:   Refinenewcp(S1, SVar, dim, F, stepBtype, degree, epsLocal, debuglevel, p); break;
            case 1:   Refinenewsimple(S1, SVar, dim, F, stepBtype, degree, epsLocal, debuglevel, p); break;
            case 2:   RefinenewsimpleT(S1, SVar, dim, F, stepBtype, degree, epsLocal, debuglevel, p); break;
            case 3:   Refinenewnotransform(S1, SVar, dim, F, stepBtype, degree, epsLocal, debuglevel, p); break;
            case 4:   Refinenewnoeuler(S1, SVar, dim, F, stepBtype, degree, epsLocal, debuglevel, p); break;
            case 5:   Refinenewnoeulerplus(S1, SVar, dim, F, stepBtype, degree, epsLocal, debuglevel, p, Hlocal); break;
            case 6:   break;
            default:  break;
            }

            Tcur = S1.T.back();
            h = Hlocal - Tcur;
        }

        const IVector ulB = S1.E.front();
        const IVector olB = S1.E.back();

        // Record each call (same meaning as your other algorithms: input box -> end enclosure).
        E0.push_back(B);
        E1.push_back(olB);

        if (!hasSbound) {
            Sbound = olB;
            hasSbound = true;
        }
        else {
            Sbound = Box(Sbound, olB);
        }

        return { ulB, olB };
    };

    auto processFaceByEndCover = [&](const IVector& faceBox, const std::string& name) {
        cout << "Processing " << name << " Face (EndCover)" << endl;
        (void)endcover::EndCover(faceBox, veps, H, endEnc);
    };

    // Build 6 faces from initial 3D box S.E[0].
    IVector BottomFace(dim);
    BottomFace[0] = S.E[0][0];
    BottomFace[1] = S.E[0][1];
    BottomFace[2] = interval(S.E[0][2].leftBound(), S.E[0][2].leftBound());

    IVector TopFace(dim);
    TopFace[0] = S.E[0][0];
    TopFace[1] = S.E[0][1];
    TopFace[2] = interval(S.E[0][2].rightBound(), S.E[0][2].rightBound());

    IVector FrontFace(dim);
    FrontFace[0] = S.E[0][0];
    FrontFace[1] = interval(S.E[0][1].leftBound(), S.E[0][1].leftBound());
    FrontFace[2] = S.E[0][2];

    IVector BackFace(dim);
    BackFace[0] = S.E[0][0];
    BackFace[1] = interval(S.E[0][1].rightBound(), S.E[0][1].rightBound());
    BackFace[2] = S.E[0][2];

    IVector LeftFace(dim);
    LeftFace[0] = interval(S.E[0][0].leftBound(), S.E[0][0].leftBound());
    LeftFace[1] = S.E[0][1];
    LeftFace[2] = S.E[0][2];

    IVector RightFace(dim);
    RightFace[0] = interval(S.E[0][0].rightBound(), S.E[0][0].rightBound());
    RightFace[1] = S.E[0][1];
    RightFace[2] = S.E[0][2];

    processFaceByEndCover(BottomFace, "Bottom");
    processFaceByEndCover(TopFace, "Top");
    processFaceByEndCover(FrontFace, "Front");
    processFaceByEndCover(BackFace, "Back");
    processFaceByEndCover(LeftFace, "Left");
    processFaceByEndCover(RightFace, "Right");

    if (!hasSbound) {
        Sbound = S.E[0];
    }

    Sdata = {
        E0,
        E1,
        Sbound
    };
}










int main(int argc, char* argv[]) {
    int iflag = 0;
    int mode = 1; // 1: TwoDim/ThreeDim boundary algo; 0: EndCover
    int method = 0;
    int stepB = 0;  // 新增
    int stepA = 0;  // 新增
    int n = 0;
    double eps = 0.0;
    int order = 0;
    double T = 0;
    int debuglevel = 0; 
    std::vector<std::string> SVar;
    std::vector<std::string> SFun;
    IVector B;

    try {
        // New CLI:
        // radicalIVP-new.exe iflag mode method stepB stepA n <vars..> <funs..> eps order T debug <intervals..>
        // mode=1 -> call TwoDimEncAlgo/ThreeDimEncAlgo; mode=0 -> call endcover::EndCover
        if (argc < 7) {
            throw std::runtime_error("Insufficient number of arguments provided. Run default example");
        }

        int argi = 1;
        iflag = std::stoi(argv[argi++]);
        mode = std::stoi(argv[argi++]);
        method = std::stoi(argv[argi++]);
        stepB = std::stoi(argv[argi++]);
        stepA = std::stoi(argv[argi++]);
        n = std::stoi(argv[argi++]);

        // check n
        if (n <= 0) {
            throw std::runtime_error("Invalid value for n. n must be a positive integer.");
        }

        if (!(mode == 0 || mode == 1)) {
            throw std::runtime_error("Invalid mode. mode must be 0 (EndCover) or 1 (Boundary algo).");
        }

        const int required = 1 + 6 + (2 * n) + 4 + (2 * n);
        if (argc < required) {
            throw std::runtime_error("Insufficient number of arguments provided for vars/funs/params/intervals.");
        }

        for (int i = 0; i < n; ++i) {
            SVar.push_back(std::string(argv[argi++]));
        }

        for (int i = 0; i < n; ++i) {
            SFun.push_back(std::string(argv[argi++]));
        }

        eps = std::stod(argv[argi++]);
        order = std::stoi(argv[argi++]);
        T = std::stod(argv[argi++]);
        debuglevel = std::stoi(argv[argi++]);

        // interval
        B.resize(n);
        for (int i = 0; i < n; ++i) {
            double lo = std::stod(argv[argi++]);
            double hi = std::stod(argv[argi++]);
            B[i] = capd::interval(lo, hi);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << "Reverting to default parameters.\n";

        // default fallback values
        iflag = 0;
        mode = 1;
        method = 0;
        stepB = 0;
        stepA = 0;
        n = 2;
        eps = 0.1;
        order = 20 ;
        T = 1;
        debuglevel = 0;
        SVar = { "x", "y" };
        SFun = { "2*x-2*x*y", "-y+x*y" };
        B.resize(n);
        B[0] = capd::interval(0.9, 1.1);
        B[1] = capd::interval(2.9, 3.1);
    }

    if (iflag == -1) {
        // ===== 从 input.txt 读取参数 =====
        cout << "data from input.txt" << endl;
        std::ifstream fin("input1.txt");
        if (!fin.is_open()) {
            throw std::runtime_error("Failed to open input.txt.");
        }

        fin >> method >> stepB >> stepA >> n;
        if (n <= 0) {
            throw std::runtime_error("Invalid value for n in input.txt.");
        }

        SVar.resize(n);
        for (int i = 0; i < n; ++i) {
            fin >> SVar[i];
        }

        SFun.resize(n);
        for (int i = 0; i < n; ++i) {
            fin >> SFun[i];
        }

        fin >> eps >> order >> T >> debuglevel;

        B.resize(n);
        for (int i = 0; i < n; ++i) {
            double lo, hi;
            fin >> lo >> hi;
            B[i] = capd::interval(lo, hi);
        }

        fin.close();
    }
    

    double veps= eps;
    double  delta =  veps/10;
    for (int i = 0; i < n; ++i) {
        SFun[i] = expand_expression(SFun[i]);

    }
    std::string sumF = Convert_to_IMap(SVar, SFun);
   int degree = order;

    IMap F1(sumF,3.0);
    IMap F(sumF,3.0);

    
   
    IMatrix JJ = computeJacobian(F, B);
    IMap finp3(Convert_to_IMap(SVar, SVar), 2.0);
    IMap fp3(Convert_to_IMap(SVar, SVar), 2.0);
    // Initialize ReturnType
    ReturnType ret = {
        SVar,   // p2
        SVar,
        finp3,
        fp3,
        {100.0},                      // mu
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
  
   
    StageBound Dat = {
            {},
        {},
        B
    };
   // double veps = 3.3;
   // if (method == 0) { delta = 0.01 * veps; }
   // IVector EE= stepBcrlohner(F, degree, T, B);
  //  cout << EE << endl;
    double t0 = 0.0;
    double h;

    IVector GE;
    ReturnType2 return1;
    double len1;
    std::vector<double> timesteprecord = { 0 };
    auto start1 = std::chrono::high_resolution_clock::now();
    Stage S1 = {
       {0.0},           // T
       {B},  // E (IVector example)
       {B},   // F (IVector example)
       {G}
    };
    Stage S2 = {
       {0.0},           // T
       {B},  // E (IVector example)
       {B},   // F (IVector example)
       {G}
    };
    Stage S3 = {
       {0.0},           // T
       {B},  // E (IVector example)
       {B},   // F (IVector example)
       {G}
    };
    if (n == 2) {
        if (mode == 1) {
            // Default behavior: run ONCE to final time T.
            TwoDimEncAlgo(Dat, SVar, SFun, F, S3, eps, delta, degree, T, stepB, stepA, debuglevel, n, method, 0);
        } else {
            std::vector<IVector> E0Boxes;
            E0Boxes.reserve(4096);

            auto endEnc = [&](const IVector& box, double epsLocal, const std::vector<double>& p, double Hlocal)
                -> std::pair<IVector, IVector> {
                Stage s = runBoxToHForEndCover(
                    box,
                    SVar,
                    SFun,
                    F,
                    G,
                    epsLocal,
                    delta,
                    degree,
                    Hlocal,
                    stepB,
                    stepA,
                    debuglevel,
                    n,
                    method,
                    p);

                E0Boxes.push_back(s.E.front());
                return { s.E.front(), s.E.back() };
            };

            endcover::EndCoverResult coverAtT;
            try {
                const int maxSplits = 2000000;
                coverAtT = endcover::EndCover(B, eps, T, endEnc, maxSplits);
            }
            catch (const std::exception& e) {
                std::cerr << "[EndCover] failed after forced subdivisions: " << e.what() << std::endl;
                std::cerr << "Suggestion: reduce eps, reduce T, or increase maxSplits in radicalIVP-new.cpp." << std::endl;
                return 3;
            }

            Dat.E0 = E0Boxes;
            Dat.E1 = coverAtT.cover;
        }
    }
    else {
        if (mode == 1) {
            ThreeDimEncAlgo(Dat, SVar, SFun, F, S, eps, delta, degree, T, stepB, stepA, debuglevel, n, method);
        } else {
            std::vector<IVector> E0Boxes;
            E0Boxes.reserve(4096);

            auto endEnc = [&](const IVector& box, double epsLocal, const std::vector<double>& p, double Hlocal)
                -> std::pair<IVector, IVector> {
                Stage s = runBoxToHForEndCover(
                    box,
                    SVar,
                    SFun,
                    F,
                    G,
                    epsLocal,
                    delta,
                    degree,
                    Hlocal,
                    stepB,
                    stepA,
                    debuglevel,
                    n,
                    method,
                    p);

                E0Boxes.push_back(s.E.front());
                return { s.E.front(), s.E.back() };
            };

            endcover::EndCoverResult coverAtT;
            try {
                const int maxSplits = 2000000;
                coverAtT = endcover::EndCover(B, eps, T, endEnc, maxSplits);
            }
            catch (const std::exception& e) {
                std::cerr << "[EndCover] failed after forced subdivisions: " << e.what() << std::endl;
                std::cerr << "Suggestion: reduce eps, reduce T, or increase maxSplits in radicalIVP-new.cpp." << std::endl;
                return 3;
            }

            Dat.E0 = E0Boxes;
            Dat.E1 = coverAtT.cover;
        }
    }

    // Snapshot the time-T cover before any optional plotting extras.
    const std::vector<IVector> E1AtT = Dat.E1;

    // Compute the minimal axis-aligned box hull of all boxes at time T.
    if (!Dat.E1.empty()) {
        IVector unionBox = Dat.E1.front();
        for (size_t i = 1; i < Dat.E1.size(); ++i) {
            unionBox = Box(unionBox, Dat.E1[i]);
        }
        Dat.E = unionBox;
    }

    auto endAll = std::chrono::high_resolution_clock::now();
    auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(endAll - start1);

    // iflag outputs are progressive: larger iflag includes all smaller outputs.
    // 0+: outputs time
    // 1+: 输出在时间T时，得到的所有盒子的最小盒子闭包
    // 2+: number of boxes in Dat.E0
    // 3+: 生成 E0.txt, E1.txt（只包含时间 T 的覆盖）
    // 4+: 额外生成 E_0.txt, E_1.txt（用于画图：E_1 包含 0.1/0.4/0.7 的跟踪结果；若 n==2 也包含角点跟踪）

    if (iflag >= 0) {
        cout << "time(ms)=" << durationMs.count() << endl;
    }

    if (iflag >= 1) {
        if (!E1AtT.empty()) {
            cout << "Hull(T)=" << Dat.E << endl;
        } else {
            cout << "Hull(T)=<empty>" << endl;
        }
    }

    if (iflag >= 2) {
        cout << "E0Boxes=" << Dat.E0.size() << endl;
    }

    if (iflag >= 3) {
        // Always write the base sets (time T only for E1) for backward compatibility.
        try { saveBoxes(Dat.E0, "E0.txt"); }
        catch (const std::exception& e) { std::cerr << "saveBoxes(E0) failed: " << e.what() << std::endl; }
        try { saveBoxes(E1AtT, "E1.txt"); }
        catch (const std::exception& e) { std::cerr << "saveBoxes(E1) failed: " << e.what() << std::endl; }
    }

    if (iflag >= 4) {
        // Build plotting outputs with intermediate tracking times.
        std::vector<IVector> plotE0 = Dat.E0;
        std::vector<IVector> plotE1 = E1AtT;

        std::vector<double> trackTimes = { 0.1, 0.4, 0.7 };
        trackTimes.erase(
            std::remove_if(trackTimes.begin(), trackTimes.end(), [&](double h) {
                return !(h > 0.0) || h > T + 1e-14;
            }),
            trackTimes.end());

        for (const auto& e0 : plotE0) {
            for (double Ht : trackTimes) {
                IVector EE = stepBcrlohner(F, degree, Ht, e0);
                plotE1.push_back(EE);
            }
        }

        if (n == 2) {
            std::vector<double> cornerTimes = trackTimes;
            if (T > 0.0) cornerTimes.push_back(T);
            cornerTimes.erase(
                std::remove_if(cornerTimes.begin(), cornerTimes.end(), [&](double h) {
                    return !(h > 0.0) || h > T + 1e-14;
                }),
                cornerTimes.end());
            std::sort(cornerTimes.begin(), cornerTimes.end());
            cornerTimes.erase(std::unique(cornerTimes.begin(), cornerTimes.end()), cornerTimes.end());

            std::vector<IVector> corners;
            corners.reserve(4);
            for (int i = 0; i < 4; ++i) {
                IVector corner(2);
                corner[0] = (i & 1) ? interval(B[0].rightBound()) : interval(B[0].leftBound());
                corner[1] = (i & 2) ? interval(B[1].rightBound()) : interval(B[1].leftBound());
                corners.push_back(corner);
            }
            plotE0.insert(plotE0.end(), corners.begin(), corners.end());
            for (const auto& c : corners) {
                for (double Ht : cornerTimes) {
                    IVector EE = stepBcrlohner(F, degree, Ht, c);
                    plotE1.push_back(EE);
                }
            }
        }

        try { saveBoxes(plotE0, "E_0.txt"); }
        catch (const std::exception& e) { std::cerr << "saveBoxes(E_0) failed: " << e.what() << std::endl; }
        try { saveBoxes(plotE1, "E_1.txt"); }
        catch (const std::exception& e) { std::cerr << "saveBoxes(E_1) failed: " << e.what() << std::endl; }
    }

   
    /*
    Extendnew(SVar, SFun, F, S, veps, delta, degree, T, stepB, stepA, debuglevel);
   // cout << S.G[1].Xform.gg << endl;
    //cout << S.E.back() << endl;
    //t0 = S.T.back();
  //  cout << "t0=" << t0 << endl;
   // cout << S.G[1].bfE[0] << endl;
    Refinenew(S, SVar, n, F, stepB, degree, veps, debuglevel);

    // cout << S.E.back() << endl;
    double htmp = 0;
    while (t0 < T)
    {
        if (t0 + 1 < T) { htmp = 1; }
        else { htmp = T - t0; }
        Extendnew(SVar, SFun, F, S, veps, delta, degree, htmp, stepB, stepA, debuglevel);
        t0 = S.T.back();
        timesteprecord.push_back(t0);

        Refinenew(S, SVar, n, F, stepB, degree, veps, debuglevel);
    }
    cout << "B0" << S.E[0] << endl;
    cout << "B=" << S.E.back() << endl;
   
   */
   
    return 0;
    }

   
