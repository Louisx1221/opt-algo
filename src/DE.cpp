#include "diff_evo.h"

de::diff_evo::diff_evo(std::function<double(dvec)>  obj,  // 目标函数
					   size_t  num_params,				  // 变量个数
                       dvec    param_min,			      // 变量下限
                       dvec    param_max,			      // 变量上限
                       size_t  np,					      // 种群大小
                       double  cr,					      //交叉概率
                       double  f,					      // 缩放因子
                       bool    dither,				      //是否随机选择每一代的缩放因子
                       double  dither_min,			      // （随机选择每一代的缩放因子情况下），缩放因子的最小值
                       double  dither_max)			      // （随机选择每一代的缩放因子情况下），缩放因子的最大值
{

    // check parameter bounds vector sizes
    if(param_min.size() != num_params || param_max.size() != num_params)
        throw std::runtime_error("param_max or param_min do not have the right size");
    
    // check parameter bounds vector values
    for(int i=0; i<num_params; ++i){
        if(param_min[i] > param_max[i])
            throw std::runtime_error("at least one element of param_min is greater than param_max");
    }

    // check population size
    if(np < 4 && np > 0)
        throw std::runtime_error("np is less than 4");

	this->obj            = obj; 
	this->num_params     = num_params;
    this->param_min      = param_min;
    this->param_max      = param_max;
    this->pop_size       = np;
    this->crossover_prob = cr;
    this->diff_weight    = f;
    this->dither         = dither;
    this->dither_min     = dither_min;
    this->dither_max     = dither_max;

    // 自动确定种群大小
    if(this->pop_size == 0){
        this->pop_size = num_params * 10;
    }

    // 在自变量约束范围内随机生成初始种群
    std::default_random_engine re;
    std::uniform_real_distribution<double> unif;

    this->population = std::vector<dvec>(pop_size);
    for(int i=0; i<pop_size; ++i)
        population[i] = dvec(num_params);

    for(int i=0; i<num_params; ++i)
	{
		unif = std::uniform_real_distribution<double>{param_min[i], param_max[i]};
        for(int j=0; j<pop_size; ++j)
		{
            population[j][i] = unif(re);		
        }	
    }
}

// 优化函数
de::dvec de::diff_evo::optimize(de::opt_mode                 mode,
                                double                       delta,
                                size_t                       max_iter)
{
    std::default_random_engine re;
    std::uniform_real_distribution<double> dither_sample(dither_min, dither_max);
    std::uniform_int_distribution<int>     pop_sample(0, pop_size - 1);
    std::uniform_int_distribution<int>     dim_sample(0, num_params - 1);
    std::uniform_int_distribution<int>     cr_sample(0, 1);
	std::uniform_real_distribution<double> unif1;

    double obj_delta = std::numeric_limits<double>::infinity();
    double prev_best = (mode == MINIMIZE) ? std::numeric_limits<double>::infinity() : -std::numeric_limits<double>::infinity();
    double best_idx = 0;

    size_t a, b, c;
    size_t r;
    dvec y(num_params);
    double cr_rnd;

    for(size_t iter=0; iter<max_iter; ++iter)
	{
        for(size_t x=0; x<pop_size; ++x)
		{  
            // pick distinct a, b, c
            do a = pop_sample(re); while(a == x);
            do b = pop_sample(re); while(b == x || b == a);
            do c = pop_sample(re); while(c == x || c == a || c == b);

            r = dim_sample(re);

            // 随机生成一个缩放因子
            if(dither)
                diff_weight = dither_sample(re);

            // 差分变异与交叉生成下一代个体 y 
            for(int i=0; i<num_params; ++i)
			{
                cr_rnd = cr_sample(re);

                if(cr_rnd < crossover_prob || i == r)
				{
                    y[i] = population[a][i] + diff_weight * (population[b][i] - population[c][i]);
				}
                else
				{ 
                    y[i] = population[x][i];
				}

				//如果新产生的个体越过边界，则重新在变量所在范围内生成随机个体
				if (y[i] > param_max[i] || y[i] < param_min[i])
				{ 
					unif1 = std::uniform_real_distribution<double>{ param_min[i], param_max[i] };
					y[i] = unif1(re);
				}			
            }

            // 计算目标函数，如果子代结果更好，则替换原有的父代
            double y_score = obj(y);
            double x_score = obj(population[x]);
            if((mode == MINIMIZE && y_score <= x_score) ||
               (mode == MAXIMIZE && y_score >= x_score))
            {
                for(int i=0; i<num_params; ++i)
                    population[x][i] = y[i];
            }
        }

        // 找到最佳解
        for(int p=0; p<pop_size; ++p){
            double score = obj(population[p]);
            
            if((mode == MINIMIZE && score < prev_best) ||
               (mode == MAXIMIZE && score > prev_best))
            {
                obj_delta = fabs(score - prev_best);
                best_idx = p;
                prev_best = score;
            }

            if(obj_delta <= delta)
                return population[best_idx];
        }
    }
    
    return population[best_idx];
}

de::diff_evo::diff_evo()//构造函数
{
}