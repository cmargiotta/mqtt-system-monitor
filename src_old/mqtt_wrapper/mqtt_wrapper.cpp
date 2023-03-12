#include "mqtt_wrapper.hpp"

using std::string;

msm::mqtt_wrapper::mqtt_wrapper(const string& user, const string& password, const string& address, const string& client_id)
    : client_(address, client_id, MAX_BUFFERED_MSGS)
{
    auto connection_options = ::mqtt::connect_options_builder()
                                  .user_name(user)
                                  .password(password)
                                  .keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD)
                                  .clean_session(true)
                                  .automatic_reconnect(true)
                                  .finalize();

    client_.connect(connection_options)->wait();
}

void msm::mqtt_wrapper::publish(const string& topic, const string& payload)
{
    if (topics_cache.find(topic) == topics_cache.end())
    {
        topics_cache.insert({topic, ::mqtt::topic(client_, topic, DEFAULT_QOS, false)});
    }

    topics_cache.at(topic).publish(payload);
}