#pragma once

#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <tuple>
#include <functional>
#include <limits>


namespace de {

    class diff_evo;

    using dvec = std::vector<double>;

    enum opt_mode {		//优化类型：最大化、最小化
        MINIMIZE,
        MAXIMIZE 
    };

    const double DEFAULT_CR         = 0.8;		//交叉概率
    const double DEFAULT_F          = 0.9;		// 缩放因子
    const double DEFAULT_DITHER_MIN = 0.5;		// （随机选择每一代的缩放因子情况下），缩放因子的最小值
    const double DEFAULT_DITHER_MAX = 1.0;		// （随机选择每一代的缩放因子情况下），缩放因子的最大值
    const double DEFAULT_DELTA      = 1e-6;			// 迭代收敛精度
    const size_t DEFAULT_MAX_ITER   = 100000;		//最大迭代次数

	

}


class de::diff_evo {

private:
	std::function<double(dvec)>  obj;  // 目标函数
    size_t             num_params;
    dvec               param_min;
    dvec               param_max;
    std::vector<dvec>  population;    // [agent][parameter]
    size_t             pop_size;
    double             crossover_prob;
    double             diff_weight;
    bool               dither;
    double             dither_min;
    double             dither_max;


public:

    diff_evo(std::function<double(dvec)>  obj,  // 目标函数
			 size_t   num_params,
             dvec     param_min,
             dvec     param_max,
             size_t   np         = 0,
             double   cr         = de::DEFAULT_CR,
             double   f          = de::DEFAULT_F,
             bool     dither     = true,
             double   dither_min = de::DEFAULT_DITHER_MIN,
             double   dither_max = de::DEFAULT_DITHER_MAX);

	diff_evo();  //构造函数


    dvec optimize(de::opt_mode                 mode     = de::MINIMIZE,
                  double                       delta    = de::DEFAULT_DELTA,
                  size_t                       max_iter = de::DEFAULT_MAX_ITER);

};