//
// Created by ukhegg on 13.04.2017.
//

#ifndef DATAGEN_LIB_BOOST_IP_ADDRESS_HPP
#define DATAGEN_LIB_BOOST_IP_ADDRESS_HPP

#include "datagen/random_source_fwd.hpp"
#include <boost/asio/ip/address.hpp>

namespace datagen
{
    template<>
    struct value_generation_algorithm<boost::asio::ip::address_v4>
    {
        boost::asio::ip::address_v4 get_random(random_source_base& r_source)
        {
            boost::asio::ip::address_v4::bytes_type bytes;
            r_source.generate_random_sequence((int8_t*)&bytes[0], bytes.size());
            return boost::asio::ip::address_v4(bytes);
        }
    };

    template<>
    struct value_generation_algorithm<boost::asio::ip::address_v6>
    {
        boost::asio::ip::address_v6 get_random(random_source_base& r_source)
        {
            boost::asio::ip::address_v6::bytes_type bytes;
            r_source.generate_random_sequence((int8_t*)&bytes[0], bytes.size());
            return boost::asio::ip::address_v6(bytes);
        }
    };

    template<>
    struct value_generation_algorithm<boost::asio::ip::address>
    {
        boost::asio::ip::address get_random(random_source_base& r_source)
        {
            if(r_source.create<bool>())
            {
                return r_source.create<boost::asio::ip::address_v4>();
            }
            else
            {
                return r_source.create<boost::asio::ip::address_v6>();
            }
        }
    };
}
#endif //DATAGEN_LIB_BOOST_IP_ADDRESS_HPP
