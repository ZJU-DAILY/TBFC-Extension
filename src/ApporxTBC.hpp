/**
 *@description: ApproxBFC + TBC
 *@author: Xinwei Cai
 *@date: Sept. 2023
 */

#include "TBC.hpp"
#include "TBCPlus.hpp"

struct ApproxTBC {
    ApproxTBC(Graph& g) : graph_(g) {}

    template<class Fun>
    std::vector<int64_t> operator()(int64_t delta, double prob, Fun&& rng, int version = 3) {
        graph_.rebuild_with_prob(prob, rng);
        std::vector<int64_t> res;
        if (version == 1) {
            TBC tbc(graph_);
            res = tbc(delta);
        } else if (version == 2) {
            TBCPlus tbcp(graph_);
            res = tbcp(delta, 0);
        } else {
            TBCPlus tbcp(graph_);
            res = tbcp(delta, 1);
        }
        double weight = (1.0 / prob) * (1.0 / prob) * (1.0 / prob) * (1.0 / prob);
        for (auto &item : res) {
            item = item * weight;
        }
        return res;
    }

    Graph& graph_;
};