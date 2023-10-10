/**
 *@description: Main file
 *@author: Xinwei Cai
 *@date: Sept. 2023
 */

#include "TBC.hpp"
#include "TBCPlus.hpp"
#include "STBC.hpp"
#include "STBCPlus.hpp"
#include "TBE.hpp"
#include "TBEPlus.hpp"
#include "MC.hpp"
#include "ApporxTBC.hpp"
#include "sGrappTBC.hpp"

/*
argv[0] = ./main
argv[1] = data_path
argv[2] = delta
argv[3] = algorithm :
         -10 : TBE (not print instance, for experiments)
         -11 : TBE (print instance)
         -20 : TBE$^+$ (not print instance, for experiments)
         -21 : TBE$^+$ (print instance)
          1 : TBC
          2 : TBC$^+$
          3 : TBC$^{++}$
          4 : STBC
          5 : STBC$^+$
          61 : ApproxTBC
          62 : ApproxTBC$^+$
          63 : ApproxTBC$^{++}$
          71 : sGrappTBC
          72 : sGrappTBC$^+$
          73 : sGrappTBC$^{++}$

(optional)
algorithm 4
argv[4] = |Window| (default: 10000)
argv[5] = |Stride|/|window| (default: 0.05)
alporithm 5
argv[4] = |Window| (default: 10000)
argv[5] = |Stride|/|window| (default: 0.05)
argv[6] = |Thread| (default: 8)
algorithms 61, 62, 63
argv[4] = sampling probability (default: 0.8)
algorithms 71, 72, 73
argv[4] = unique timestamps per window (default: 1000)
argv[5:10] = approximation exponent theta_i (default: 1.05)
*/

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 11) {
        std::cerr << "Error: wrong args!\n";
        exit(EXIT_FAILURE);
    }
    std::mt19937 seed(std::random_device{}());
    auto rand_fun = [&]() -> double {
        return std::uniform_real_distribution<double>{}(seed);
    };
    std::string data_path = argv[1];
    int64_t delta = std::stoll(argv[2]);
    int algo_id = std::stoi(argv[3]);
    auto run_tbe_noprint = [&]() {
        Graph g;
        int64_t TS = g.load(data_path);
        TBE tbe(g);
        auto ti_st = Time::now();
        auto res = tbe(delta);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_tbe = [&]() {
        Graph g;
        int64_t TS = g.load(data_path);
        TBE tbe(g, true);
        auto ti_st = Time::now();
        auto res = tbe(delta);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_tbep_noprint = [&]() {
        Graph g;
        int64_t TS = g.load(data_path);
        TBEPlus tbe(g);
        auto ti_st = Time::now();
        auto res = tbe(delta);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_tbep = [&]() {
        Graph g;
        int64_t TS = g.load(data_path);
        TBEPlus tbe(g, true);
        auto ti_st = Time::now();
        auto res = tbe(delta);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_tbc = [&]() {
        Graph g;
        int64_t TS = g.load(data_path);
        TBC tbc(g);
        auto ti_st = Time::now();
        auto res = tbc(delta);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_tbcp = [&]() {
        Graph g;
        int64_t TS = g.load(data_path);
        TBCPlus tbcp(g);
        auto ti_st = Time::now();
        auto res = tbcp(delta, 0);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_tbcpp = [&]() {
        Graph g;
        int64_t TS = g.load(data_path);
        TBCPlus tbcpp(g);
        auto ti_st = Time::now();
        auto res = tbcpp(delta, 1);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_stbc = [&]() {
        int window_size = 10000;
        if (argc >= 5) {
            window_size = std::stoi(argv[4]);
        }
        int stride_size = 0.05 * window_size;
        if (argc >= 6) {
            stride_size = std::stod(argv[5]) * window_size;
        }
        StreamGraph sg;
        int64_t TS = sg.load(data_path);
        STBC stbc(sg, delta);
        auto ti_st = Time::now();
        stbc.shift_window(window_size, stride_size);
        auto ti_en = Time::now();
        time_cost(ti_st, ti_en);
    };
    auto run_stbcp = [&]() {
        int window_size = 10000;
        if (argc >= 5) {
            window_size = std::stoi(argv[4]);
        }
        int stride_size = 0.05 * window_size;
        if (argc >= 6) {
            stride_size = std::stod(argv[5]) * window_size;
        }
        int thread_size = 8;
        if (argc == 7) {
            thread_size = std::stoi(argv[6]);
        }
        StreamGraph sg;
        int64_t TS = sg.load(data_path);
        STBCPlus stbcp(sg, delta, thread_size);
        auto ti_st = Time::now();
        stbcp.shift_window(window_size, stride_size);
        auto ti_en = Time::now();
        time_cost(ti_st, ti_en);
    };
    auto run_approx_tbc = [&](int version) {
        double prob = 0.8;
        if (argc >= 5) {
            prob = std::stod(argv[4]);
        }
        Graph g;
        int64_t TS = g.load(data_path, true);
        ApproxTBC approx_tbc(g);
        auto ti_st = Time::now();
        auto res = approx_tbc(delta, prob, rand_fun, version);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };
    auto run_sgrapp_tbc = [&](int version) {
        int32_t uni_time_per_window = 1000;
        if (argc >= 5) {
            uni_time_per_window = std::stoi(argv[4]);
        }
        std::vector<double> theta(TYPE_SIZE, 1.05);
        if (argc >= 11) {
            for (int i = 0; i < TYPE_SIZE; ++i) {
                theta[i] = std::stod(argv[i + 5]);
            }
        }

        StreamGraph sg;
        int64_t TS = sg.load(data_path);
        sGrappTBC sgrapp_tbc(sg, delta);
        auto ti_st = Time::now();
        auto res = sgrapp_tbc(uni_time_per_window, theta, version);
        auto ti_en = Time::now();
        std::cout << res << '\n';
        time_cost(ti_st, ti_en);
    };

    switch (algo_id) {
    case -10 : run_tbe_noprint(); break;
    case -11 : run_tbe(); break;
    case -20 : run_tbep_noprint(); break;
    case -21 : run_tbep(); break;
    case 1 : run_tbc(); break;
    case 2 : run_tbcp(); break;
    case 3 : run_tbcpp(); break;
    case 4 : run_stbc(); break;
    case 5 : run_stbcp(); break;
    case 61 :
    case 62 :
    case 63 : run_approx_tbc(algo_id % 10); break;
    case 71 :
    case 72 :
    case 73 : run_sgrapp_tbc(algo_id % 10); break;
    default:
        std::cerr << "Error: wrong algorithm!\n";
        break;
    }
}