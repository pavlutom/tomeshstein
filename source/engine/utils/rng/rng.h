//
// Created by tomesh on 1/16/19.
//

#ifndef TOMESH_RNG_H
#define TOMESH_RNG_H

#include <random>

namespace rng {
    class __Rng {   // SINGLETON
    public:
        __Rng(__Rng const &)            = delete;
        void operator=(__Rng const &)   = delete;

    private:
        inline static __Rng & getInstance() { static __Rng instance; return instance; }

        template<typename _T, typename ...Args> inline friend _T uniform_real   (Args && ...args);
        template<typename _T, typename ...Args> inline friend _T uniform_int    (Args && ...args);

        __Rng() : m_Engine ( [](){ std::random_device rd; return rd(); }() ) {}

        std::mt19937_64 m_Engine;
    };

    template<typename _T, typename ...Args>
    inline _T uniform_real(Args && ...args) { std::uniform_real_distribution<_T> dist(args...); return dist(__Rng::getInstance().m_Engine); }
    template<typename _T, typename ...Args>
    inline _T uniform_int(Args && ...args)  { std::uniform_int_distribution<_T> dist(args...);  return dist(__Rng::getInstance().m_Engine); }
}

#endif //TOMESH_RNG_H
