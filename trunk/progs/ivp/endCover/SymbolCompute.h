#include <iostream>


#include <symengine/basic.h>
#include <symengine/add.h>
#include <symengine/symbol.h>
#include <symengine/dict.h>
#include <symengine/integer.h>
#include <symengine/mul.h>
#include <symengine/pow.h>
#include <symengine/parser.h>

using SymEngine::Add;
using SymEngine::Basic;
using SymEngine::Integer;
using SymEngine::integer;
using SymEngine::Mul;
using SymEngine::multinomial_coefficients;
using SymEngine::Pow;
using SymEngine::RCP;
using SymEngine::rcp_dynamic_cast;
using SymEngine::Symbol;
using SymEngine::symbol;
using SymEngine::umap_basic_num;
std::string expand_expression(const std::string &input)
{
    // 替换运算符格式
    std::string adjusted = input;
    size_t pos = 0;

    // 预处理输入字符串
    while ((pos = adjusted.find('^', pos)) != std::string::npos) {
        adjusted.replace(pos, 1, "**");
        pos += 2;
    }
    while ((pos = adjusted.find(' ', pos)) != std::string::npos) {
        adjusted.erase(pos, 1); // 删除空格
    }

    try {
        // 解析表达式
        RCP<const Basic> expr = SymEngine::parse(adjusted);

        // 展开表达式
        RCP<const Basic> expanded = expand(expr);

        // 转换为字符串并后处理
        std::string result = expanded->__str__();

        // 格式化输出
        pos = 0;
        while ((pos = result.find("**", pos)) != std::string::npos) {
            result.replace(pos, 2, "^");
            pos += 1;
        }

        // 添加乘法符号（可选增强）
        pos = 0;
        while ((pos = result.find_first_of("xyzwabcdefghijklmnopqrstuv", pos))
               != std::string::npos) {
            if (pos > 0 && isalnum(result[pos - 1])) {
                result.insert(pos, "*");
                pos += 2;
            } else {
                pos += 1;
            }
        }

        return result;

    } catch (const std::exception &e) {
        return "Error: " + std::string(e.what());
    } catch (...) {
        return "Error: Invalid expression";
    }
}

// 使用示例

  //  std::vector<std::string> SFun = { "((x^(-1))^2*(-1)*(x^(2)))", "(x+y)^3" };

   // for (int i = 0; i < 2; ++i) {
  //      SFun[i] = expand_expression(SFun[i]);
   //     cout << "SFun" << SFun[i] << endl;
   // 



