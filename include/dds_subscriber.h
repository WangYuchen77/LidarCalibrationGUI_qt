#ifndef DDS_SUBSCRIBER_H_
#define DDS_SUBSCRIBER_H_

#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/Domain.h>
#include <fastrtps/subscriber/SampleInfo.h>

namespace cmdrDDS
{
template <typename T, typename M>
class DdsSubscriber
{
public:
    DdsSubscriber() : mp_subscriber_(nullptr), mp_participant_(nullptr){};
    virtual ~DdsSubscriber()
    {
        eprosima::fastrtps::Domain::removeSubscriber(mp_subscriber_);
    };
    /**
     * !!! NOTE
     * This is an overloaded legacy creating function. 
     * Input argument datatype is not needed. Use other functions for future consideration. 
     * @param DdsParticipant &participant_ref  Reference of participant.
     * @param const std::string &datatype      Topictype, string (No Influence).
     * @param const std::string &topicname     Topicname, string.
     * @param uint32_t history                 Depth of message.
     * @param function &callback_ptr           Callback function potiner.
     * @return True if success.
     * */
    bool Create(DdsParticipant &participant_ref,
                const std::string &datatype,
                const std::string &topicname,
                uint32_t history,
                const std::function<void(T *)> &callback_ptr)
    {
        return (Create(participant_ref, topicname, history, callback_ptr));
    }
    /**
     * Implementation of subscriber creation. 
     * @param DdsParticipant &participant_ref  Reference of participant.
     * @param const std::string &topicname     topicname, string.
     * @param uint32_t history                 Depth of message.
     * @param function &callback_ptr           Callback function potiner.
     * @return True if success.
     * */
    bool Create(DdsParticipant &participant_ref,
                const std::string &topicname,
                uint32_t history,
                const std::function<void(T *)> &callback_ptr)
    {
        if (mp_subscriber_ != nullptr)
        {
            std::cout << "Subscriber already created.\n";
            return false;
        }

        eprosima::fastrtps::TopicDataType *tmptype = nullptr;
        mp_participant_ = participant_ref.get_participant_ptr();
        if (eprosima::fastrtps::Domain::getRegisteredType(mp_participant_, m_type_.getName(), &tmptype))
        {
            //std::cout << "already exists";
        }
        else
            eprosima::fastrtps::Domain::registerType(mp_participant_, &m_type_);

        m_listener_.callback_pointer_ = callback_ptr;
        Rparam.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
        Rparam.topic.topicDataType = m_type_.getName();
        Rparam.topic.topicName = topicname;
        Rparam.topic.historyQos.kind = eprosima::fastrtps::KEEP_LAST_HISTORY_QOS;
        Rparam.topic.historyQos.depth = history;
        Rparam.topic.resourceLimitsQos.max_samples = 200;
        Rparam.qos.m_reliability.kind = eprosima::fastrtps::BEST_EFFORT_RELIABILITY_QOS;
        Rparam.qos.m_durability.kind = eprosima::fastrtps::VOLATILE_DURABILITY_QOS;
        mp_subscriber_ = eprosima::fastrtps::Domain::createSubscriber(mp_participant_,
                                                                      Rparam,
                                                                      (eprosima::fastrtps::SubscriberListener *)&m_listener_);
        if (mp_subscriber_ == nullptr)
            return false;

        return true;
    }
    /**
     * Implementation of subscriber creation with XML file. 
     * @param DdsParticipant &participant_ref       Reference of participant.
     * @param const std::string &xml_profile_name   Input XML profile.
     * @param function &callback_ptr                Callback function potiner.
     * @return True if success.
     * */
    bool Create(DdsParticipant &participant_ref,
                const std::string &xml_profile_name,
                const std::function<void(T *)> &callback_ptr)
    {
        mp_participant_ = participant_ref.get_participant_ptr();
        eprosima::fastrtps::TopicDataType *tmptype = nullptr;
        if (eprosima::fastrtps::Domain::getRegisteredType(mp_participant_, xml_profile_name.c_str(), &tmptype))
        {
            //std::cout << "already exists";
        }
        else
            eprosima::fastrtps::Domain::registerType(mp_participant_, &m_type_);

        m_listener_.callback_pointer_ = callback_ptr;

        mp_subscriber_ = eprosima::fastrtps::Domain::createSubscriber(mp_participant_,
                                                                      xml_profile_name,
                                                                      (eprosima::fastrtps::SubscriberListener *)&m_listener_);
        if (mp_subscriber_ == nullptr)
            return false;

        return true;
    }

    /**
     * Implementation of detach function for a subscriber.
     * The create function should be called before detach. 
     * @return True if success.
     * */
    bool detach()
    {
        if (mp_participant_ == nullptr || mp_subscriber_ == nullptr)
        {
            std::cout << "Call method error in "
                      << __FILE__ << "," << __LINE__
                      << ". You are supposed to CREATE before a DETACH." << std::endl;
            return false;
        }

        return (eprosima::fastrtps::Domain::removeSubscriber(mp_subscriber_));
    }

    /**
     * Implementation of rejoin function for a subscriber. 
     * First the subscriber will be removed in the participant, 
     * then rejoin the participant with the attributes defined when created.
     * @return True if success.
     * */
    bool rejoin()
    {
        if (mp_participant_ == nullptr || mp_subscriber_ == nullptr)
        {
            std::cout << "Call method error in "
                      << __FILE__ << "," << __LINE__
                      << ". You are supposed to CREATE before a REJOIN." << std::endl;
            return false;
        }

        detach();
        mp_subscriber_ = eprosima::fastrtps::Domain::createSubscriber(mp_participant_,
                                                                      Rparam,
                                                                      (eprosima::fastrtps::SubscriberListener *)&m_listener_);
        return true;
    }

    /**
     * Implementation of setTopicName function for a subscriber. 
     * The topic name attribute is set to name and calls rejoin to update.
     * @return True if success.
     * */
    bool setTopicName(const std::string &name)
    {
        if (mp_participant_ == nullptr || mp_subscriber_ == nullptr)
        {
            std::cout << "Call method error in "
                      << __FILE__ << "," << __LINE__
                      << ". You are supposed to call CREATE first." << std::endl;
            return false;
        }

        Rparam.topic.topicName = name;
        return (rejoin());
    }

    /**
     * Set callback manually. 
     * @return True if success.
     * */
    bool setCallback(const std::function<void(T *)> &callback_ptr)
    {
        if (callback_ptr == nullptr)
            return false;

        m_listener_.callback_pointer_ = callback_ptr;
        return true;
    }

private:
    eprosima::fastrtps::Subscriber *mp_subscriber_;
    eprosima::fastrtps::SubscriberAttributes Rparam;
    eprosima::fastrtps::Participant *mp_participant_;
    M m_type_;

public:
    class SubListener : public eprosima::fastrtps::SubscriberListener
    {
    public:
        SubListener(){};
        ~SubListener(){};
        void onNewDataMessage(eprosima::fastrtps::Subscriber *sub)
        {
            if (sub->takeNextData((void *)&m_data_, &m_info_))
            {
                if (m_info_.sampleKind == eprosima::fastrtps::rtps::ALIVE)
                {
                    callback_pointer_(&m_data_);
                }
            }
        }
        eprosima::fastrtps::SampleInfo_t m_info_;
        T m_data_;
        std::function<void(T *)> callback_pointer_;
    } m_listener_;
};
}; // namespace cmdrDDS

#endif /*end of DDS_SUBSCRIBER_H_*/
