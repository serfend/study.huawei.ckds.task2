
#include <iostream>
#include <map>
#include <vector>
#include <cstring>
//#include <random>
//#include "math.h"
// #include "stdlib.h"
#define DEBUG true
#define DEBUGSUBMIT true
#define DEBUGRESULT true
#if DEBUG
#include <windows.h>
#endif

std::string user_func_to_string(int n)
{
    int m = n;
    int i = 0, j = 0;
    char s[100];
    char ss[100];
    while (m > 0)
    {
        s[i++] = m % 10 + '0';
        m /= 10;
    }
    s[i] = '\0';

    i = i - 1;
    while (i >= 0)
    {
        ss[j++] = s[i--];
    }
    ss[j] = '\0';

    return ss;
}
double user_func_min(double a, double b)
{
    return a > b ? b : a;
}
double user_func_abs(double a)
{
    return a < 0 ? -a : a;
}
class Source;
class Pos;
class Postman;
class Repository;
class Solution;
struct effectionest
{
public:
    Source *target;
    double effection;
    effectionest(Source *target, double effection);
};
class Pos
{
public:
    int x;
    int y;
    Pos()
    {
        this->x = 0;
        this->y = 0;
    }
    Pos(int x, int y) : x(x), y(y)
    {
    }
    void set_pos(Pos *pos)
    {
        this->x = pos->x;
        this->y = pos->y;
    }
    int distance(Pos *target)
    {
        return this->distance(target->x, target->y);
    }
    int distance(int x, int y)
    {
        double d = user_func_abs((double)this->x - (double)x) + user_func_abs((double)this->y - (double)y);
        return d * d;
    }
    Pos* clone() {
        return new Pos(this->x, this->y);
    }
#if DEBUG
    std::string to_string()
    {
        return user_func_to_string(this->x) + "," + user_func_to_string(this->y);
    }
#endif
};

class Source
{
public:
    int value;
    Pos *pos;
    Source(Pos *pos, int value) : pos(pos), value(value)
    {
    }
    double distance(Source *target)
    {
        double d = (double)(this->pos->distance(target->pos));
        if (d < 0.01)
            d = 0.01;
        return d;
    }
#if DEBUG
    std::string to_string()
    {
        return "v:" + user_func_to_string(this->value) + "pos:" + this->pos->to_string();
    }
#endif
};
effectionest::effectionest(Source *target, double effection)
{
    this->target = target;
    this->effection = effection;
}
class Postman : public Source
{
public:
    int max_value;
    Postman(Pos *pos, int value, int max_value) : Source(pos, value)
    {
        this->max_value = max_value;
    };
    std::vector<int> navigate(Source *target, int **infomap)
    {
        int dis_x = this->pos->x - target->pos->x;
        int dis_y = this->pos->y - target->pos->y;
        int abs_dis_x = user_func_abs(dis_x);
        int abs_dis_y = user_func_abs(dis_y);
        bool dis_x_active = abs_dis_x > 0;
        bool dis_y_active = abs_dis_y > 0;
        std::vector<int> result = std::vector<int>();
        if (dis_x_active && dis_y_active)
        {
            int x_ray = dis_x < 0 ? 1 : -1;
            int y_ray = dis_y < 0 ? 3 : -3;
            std::vector<int> vx = std::vector<int>(abs_dis_x, x_ray);
            result.insert(result.end(), vx.begin(), vx.end());
            std::vector<int> vy = std::vector<int>(abs_dis_y, y_ray);
            result.insert(result.end(), vy.begin(), vy.end());
            //std::shuffle(result.begin(), result.end(), std::default_random_engine());
        }
        else if (dis_x_active)
        {
            int x_ray = dis_x < 0 ? 1 : -1;
            std::vector<int> vx = std::vector<int>(abs_dis_x, x_ray);
            result.insert(result.end(), vx.begin(), vx.end());
        }
        else if (dis_y_active)
        {
            int y_ray = dis_y < 0 ? 3 : -3;
            std::vector<int> vy = std::vector<int>(abs_dis_y, y_ray);
            result.insert(result.end(), vy.begin(), vy.end());
        }
        return result;
    }
    void move(int move, Solution *glo);
    void judge(int move, Solution *glo);
    Postman* clone() {
        return new Postman(this->pos->clone(), this->value, this->max_value);
    }
#if DEBUG
    std::string to_string()
    {
        return "v:" + user_func_to_string(this->value) + "pos:" + this->pos->to_string();
    }
#endif // DEBUG
};

class Repository : public Source
{
public:
    Repository(Pos *pos, int value) : Source::Source(pos, value){};
    effectionest *caculate_rate(Solution *glo);
    Repository* clone() {
        return new Repository(this->pos->clone(), this->value);
    }
};

class Solution
{
public:
    Source *target;
    int map_size;
    int total_step;
    int **info_map;
    Repository *info_home;
    Postman *postman;
    std::vector<Repository *> info_repository;
    int info_repository_len() { return this->info_repository.size(); };
    std::map<std::string, Repository *> info_repository_dict;
    std::vector<Repository *> info_donator;
    int info_donator_len() { return this->info_donator.size(); };
    std::map<std::string, Repository *> info_donator_dict;
    std::vector<int> step_group;
    int now_step;
    int step_group_len() { return this->step_group.size(); };
    
    int assess_to_finnal_step(std::vector<Repository*>repository,Postman* postman,std::vector<Pos*> path){
        // deep copy
        std::vector<Repository*> rep;
        Postman* p = postman->clone();
        int min_step = 1e5;
        std::vector<Pos*> min_path;
        do {
            Repository* r = (*repository.end())->clone();
            rep.push_back(r);
            repository.pop_back();
        } while (!repository.empty());
        // iterate
        for (int i = 0; i < rep.size(); i++) {
            Repository* current = rep[i];
            if (current->value > 0 && p->value == p->max_value)continue;
            int current_step = assess_to_finnal_step(rep, p, path);
            if (current_step < min_step) {
                min_step = current_step;
                min_path = path;
            }
        }
        return min_step;
    }
    Solution()
    {
        this->map_size = 12;
        this->total_step = 0;
        this->now_step = 0;
        this->target = new Source(new Pos(), 0);
        this->total_step = 0;
        this->info_map = new int *[this->map_size];
        for (int i = 0; i < this->map_size; ++i)
        {
            this->info_map[i] = new int[this->map_size];
            std::memset(this->info_map[i], 0, this->map_size * sizeof(int));
        }

        char s;
        int x, y, v;
        std::cin >> s >> x >> y;
        Pos *pos = new Pos(x, y);
        Pos *home_pos = new Pos(x, y);
        this->info_home = new Repository(home_pos, (int)(1e4 - 1));
        this->postman = new Postman(pos, 100, 100);
        this->info_map[this->postman->pos->y][this->postman->pos->x] = (int)(1e4 - 1);
        for (int i = 0; i < 5; i++)
        {
            std::cin >> s >> x >> y >> v;
            pos = new Pos(x, y);
            Repository *r = new Repository(pos, v);
            this->info_map[pos->y][pos->x] += r->value;
            if (this->info_map[pos->y][pos->x] < 0)
            {
                std::string key = user_func_to_string(pos->y) + "|" + user_func_to_string(pos->x);

                this->info_repository_dict[key] = r;
                this->info_repository.push_back(r);
            }
        }
    }
    bool complete_all()
    {
        return false;
    }
    void navigate()
    {
        this->now_step = 0;
        this->step_group = this->postman->navigate(this->target, this->info_map);
        if (this->step_group_len() == 0)
        {
            this->postman->judge(0, this);
        }
    }
    bool assess_next(bool complete_single)
    {
        int r_len = this->info_repository_len();
        this->postman->judge(0, this);
        if (r_len == 0)
        {
            return this->complete_all();
        }
        effectionest *effectionest_put = this->get_effectionest_put(this->info_repository);
        effectionest *effectionest_get = this->get_effectionest_get(this->info_donator, this->info_home);
        this->target = effectionest_get->effection > effectionest_put->effection ? effectionest_get->target : effectionest_put->target;
        this->navigate();
        return true;
    }
    char translate(int move)
    {
        switch (move)
        {
        case 1:
            return 'S';
        case -1:
            return 'N';
        case -3:
            return 'W';
        case 3:
            return 'E';
        default:
            return 'U';
        }
    }
    bool go_next()
    {
        if (this->now_step >= this->step_group_len())
        {
            if (!this->assess_next(true))
                return false;
            return this->go_next();
        }
        int move = this->step_group[this->now_step];
#if DEBUGSUBMIT
        std::cout << this->translate(move) << std::endl;
#endif
        this->postman->move(move, this);
        this->now_step++;
        if (this->now_step >= this->step_group_len())
        {
            return this->assess_next(true);
        }
        return true;
    }
    bool start_game()
    {
        while (true)
        {
            char s;
            int posx, posy, val;
            std::cin >> s;
            switch (s)
            {
            case 'G':
            {
                if (!this->go_next())
                    return true;
                continue;
            }
            case 'R':
            {
                std::cin >> posx >> posy >> val;
                Pos *pos = new Pos(posx, posy);
                if (this->info_map[posy][posx] < 1e3)
                {
                    this->info_map[posy][posx] += val;
                    Repository *r = new Repository(pos, val);
                    std::string key = user_func_to_string(posy) + "|" + user_func_to_string(posx);
                    if (this->info_donator_dict.find(key) == this->info_donator_dict.end())
                    {
                        this->info_donator.push_back(r);
                        this->info_donator_dict[key] = r;
                    }
                    if (!this->assess_next(false))
                        return true;
                }
                continue;
            }
            default:
                return this->go_next();
            }
        }
    }
    template <typename T>
    effectionest *get_effectionest_get(std::vector<T *> arr, T *init_item)
    {
        Postman *p = this->postman;
        int p_v = p->value;
        int c = p->max_value - p_v;
        double max_effect = user_func_min(c, init_item->value) / p->distance(init_item);
        Source *max_effect_index = init_item;
        int arr_size = arr.size();
        for (int i = 0; i < arr_size; i++)
        {
            Source *r = arr[i];
            double d = user_func_min(c, r->value) / p->distance(r);
            if (d > max_effect)
            {
                max_effect = d;
                max_effect_index = r;
            }
        }
        return new effectionest(max_effect_index, max_effect);
    }
    template <typename T>
    effectionest *get_effectionest_put(std::vector<T *> arr)
    {
        Postman *p = this->postman;
        int p_v = p->value;
        double max_effect = 0;
        int arr_size = arr.size();
        // if only left 1 repository,go for that
        if (arr_size == 1) {
            Source* r = arr[0];
            if (p_v + r->value >= 0)return new effectionest(r, 1e5);
        }
        Source *max_effect_index = arr[0];
        for (int i = 0; i < arr_size; i++)
        {
            Source *r = arr[i];
            double dis = p->distance(r);
            bool check = (p_v > -r->value || p_v == p->max_value);
            double d = (check ? 3*(double)p->max_value : 2 * user_func_min(p_v, -r->value)) / dis;
            if (d > max_effect)
            {
                max_effect = d;
                max_effect_index = r;
            }
        }
        return new effectionest(max_effect_index, max_effect);
    }
#if DEBUG
    void debug_map()
    {
        Postman *p = this->postman;
        Pos *pos = p->pos;
        Pos *target = this->target->pos;
        for (int x = 0; x < this->map_size; x++)
        {
            std::cout << '|' << std::endl
                      << '|';
            for (int y = 0; y < this->map_size; y++)
            {
                if (target->distance(x, y) < 1e-3)
                    std::cout << "★";
                if (pos->distance(x, y) < 1e-3)
                    std::cout << "●" << p->value;
                else
                    std::cout << info_map[y][x];
                std::cout << '\t';
            }
            std::cout << std::endl
                      << '|' << std::endl;
        }
        Sleep(500);
    }
#endif
};
void Postman::move(int move, Solution *glo)
{
    glo->total_step++;
    Pos *pos = this->pos;
    int dy = (int)(move / 3);
    pos->y += dy;
    pos->x += (move - 3 * dy);
    this->judge(move, glo);
#if DEBUG
    glo->debug_map();
#endif
};
void Postman::judge(int move, Solution *glo)
{
    Pos *pos = this->pos;
    int **info_map = glo->info_map;
    int v = info_map[pos->y][pos->x];
    if (v > 1e-3 && this->value < this->max_value)
    {
        int delta = user_func_min(v, (double)this->max_value - (double)this->value);
        this->value += delta;
        std::string key = user_func_to_string(pos->y) + "|" + user_func_to_string(pos->x);
#if DEBUG
        std::cout << "get " + user_func_to_string(delta) + " from donator" << std::endl;
#endif
        if (v < 1e3)
        {
            info_map[pos->y][pos->x] -= delta;
            Repository *d = glo->info_donator_dict[key];
            d->value -= delta;
            std::vector<Repository *> *v = &glo->info_donator;
#if DEBUG
            std::cout << "donator:" + d->to_string() << std::endl;
#endif
            for (std::vector<Repository *>::iterator it = v->begin(); it != v->end(); it++)
            {
                if (*it == d)
                {
                    v->erase(it);
#if DEBUG
                    std::cout << "remove donator" + d->to_string() << std::endl;
#endif
                    break;
                }
            }
        }
    }
    else if (v < -1e-3 && this->value > 0)
    {
        int delta = user_func_min(user_func_abs(v), this->value);
        this->value -= delta;
        info_map[pos->y][pos->x] += delta;
        std::string key = user_func_to_string(pos->y) + "|" + user_func_to_string(pos->x);
        Repository *d = glo->info_repository_dict[key];
        d->value += delta;
#if DEBUG
        std::cout << "put " + user_func_to_string(delta) + " to repository " + d->to_string() << std::endl;
#endif
        std::vector<Repository *> *v = &glo->info_repository;
        if (d->value > -1e-3)
        {
            for (std::vector<Repository *>::iterator it = v->begin(); it != v->end(); it++)
            {
                if (*it == d)
                {
                    v->erase(it);
#if DEBUG
                    std::cout << "remove repository" + d->to_string() << std::endl;
#endif
                    break;
                }
            }
        }
    }
}
effectionest *Repository::caculate_rate(Solution *glo)
{
    Postman *postman = glo->postman;
    int v = postman->value;
    int need_v = user_func_abs(this->value);
    int c = postman->max_value;
    if (v >= need_v || v > c - 1e-2)
    {
        return new effectionest(this, 5 * (double)v / postman->distance(this));
    }
    else
    {
        effectionest *effectionest_put = glo->get_effectionest_put(glo->info_repository);
        effectionest *effectionest_get = glo->get_effectionest_get(glo->info_donator, glo->info_home);
        return effectionest_put->effection > effectionest_get->effection ? effectionest_put : effectionest_get;
    }
}
int main()
{
    Solution s;
    s.start_game();
#if DEBUGRESULT
    std::cout << "complete single task:" << user_func_to_string(s.total_step) << std::endl;
#endif // DEBUG
    return 0;
}
