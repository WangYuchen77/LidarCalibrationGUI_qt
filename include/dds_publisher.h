#ifndef DDS_PUBLISHER_H_
#define DDS_PUBLISHER_H_

#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/publisher/PublisherListener.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/Domain.h>
#include <thread>

namespace cmdrDDS
{
typedef enum
{
    ASYNC_PUBLISH_MODE,
    SYNC_PUBLISH_MODE
} ePublishMode;
template <typename T, typename M>
class DdsPublisher
{

public:
    DdsPublisher() : mp_publisher_(nullptr), mp_participant_(nullptr){};
    virtual ~DdsPublisher()
    {
        eprosima::fastrtps::Domain::removePublisher(mp_publisher_);
    };

    /**
     * !!! NOTE
     * This is an overloaded legacy creating function.
     * Input argument datatype is not needed. Use other functions for future consideration. 
     * @param DdsParticipant &participant_ref  Reference of participant.
     * @param const std::string &datatype      Topictype, string (No Influence).
     * @param const std::string &topicname     Topicname, string.
     * @param uint32_t history                 Depth of message.
     * @param const int PublishMode            PublishMode of publisher.
     *        SYNC_PUBLISH_MODE    Only when message size <= 64Kb
     *        ASYNC_PUBLISH_MODE   When message size >64Kb, this mode should be used, or message will be missing.
     * */
    bool Create(DdsParticipant &participant_ref,
                const std::string &datatype,
                const std::string &topicname,
                uint32_t history,
                const int PublishMode = SYNC_PUBLISH_MODE)
    {
        return Create(participant_ref, topicname, history, PublishMode);
    }

    /**
     * Implementation of publisher creation. 
     * @param DdsParticipant &participant_ref  Reference of participant.
     * @param const std::string &topicname     topicname, string.
     * @param uint32_t history                 Depth of message.
     * @param const int PublishMode            PublishMode of publisher.
     *        SYNC_PUBLISH_MODE    Only when message size <= 64Kb
     *        ASYNC_PUBLISH_MODE   When message size >64Kb, this mode should be used, or message will be missing.
     * */
    bool Create(DdsParticipant &participant_ref,
                const std::string &topicname,
                uint32_t history,
                const int PublishMode = SYNC_PUBLISH_MODE)
    {
        eprosima::fastrtps::TopicDataType *tmptype = nullptr;
        mp_participant_ = participant_ref.get_participant_ptr();
        if (eprosima::fastrtps::Domain::getRegisteredType(mp_participant_, m_type_.getName(), &tmptype))
        {
            //std::cout << "already exists";
        }
        else
            eprosima::fastrtps::Domain::registerType(mp_participant_, &m_type_);

        eprosima::fastrtps::PublisherAttributes Wparam;
        Wparam.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
        Wparam.topic.topicDataType = m_type_.getName();
        Wparam.topic.topicName = topicname;
        Wparam.topic.historyQos.kind = eprosima::fastrtps::KEEP_LAST_HISTORY_QOS;
        Wparam.topic.historyQos.depth = history;
        Wparam.topic.resourceLimitsQos.max_samples = 200;
        Wparam.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;
        if (PublishMode == ASYNC_PUBLISH_MODE)
            Wparam.qos.m_publishMode.kind = eprosima::fastrtps::ASYNCHRONOUS_PUBLISH_MODE;
        mp_publisher_ = eprosima::fastrtps::Domain::createPublisher(mp_participant_,
                                                                    Wparam,
                                                                    (eprosima::fastrtps::PublisherListener *)&m_listener_);
        if (mp_publisher_ == nullptr)
            return false;

        return true;
    }

    /**
     * Implementation of publisher creation with xml file.
     * @param DdsParticipant &participant_ref         Reference of participant.
     * @param const std::string &xml_profile_name     Input XML profile.
     * */
    bool Create(DdsParticipant &participant_ref,
                const std::string &xml_profile_name)
    {
        eprosima::fastrtps::TopicDataType *tmptype = nullptr;
        mp_participant_ = participant_ref.get_participant_ptr();
        if (eprosima::fastrtps::Domain::getRegisteredType(mp_participant_, xml_profile_name.c_str(), &tmptype))
        {
            //std::cout << "already exists";
        }
        else
            eprosima::fastrtps::Domain::registerType(mp_participant_, &m_type_);

        mp_publisher_ = eprosima::fastrtps::Domain::createPublisher(mp_participant_,
                                                                    xml_profile_name,
                                                                    (eprosima::fastrtps::PublisherListener *)&m_listener_);
        if (mp_publisher_ == nullptr)
            return false;

        return true;
    }
    /**
     * Write data to the topic using FastRTPS publish method in Publisher.h.
     * @param Data Pointer to the data
     * @return True if correct 
     */
    bool PublishDds(T *t_ptr)
    {
        return (mp_publisher_->write((void *)t_ptr));
    }

private:
    eprosima::fastrtps::Publisher *mp_publisher_;
    eprosima::fastrtps::Participant *mp_participant_;
    M m_type_;
    class PubListener : public eprosima::fastrtps::PublisherListener
    {
    public:
        PubListener(){};
        ~PubListener(){};
        void onPublicationMatched(eprosima::fastrtps::Publisher *pub, eprosima::fastrtps::rtps::MatchingInfo &info){};
    } m_listener_;
};
}; // namespace cmdrDDS

#endif /*end of DDS_PUBLISHER_H_*/