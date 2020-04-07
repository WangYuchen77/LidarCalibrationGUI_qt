#ifndef DDS_PARTICIPANT_H_
#define DDS_PARTICIPANT_H_

#include <string>
#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/Domain.h>
#include <fastrtps/utils/eClock.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>
// #include <fastrtps/xmlparser/XMLProfileManager.h>

namespace cmdrDDS
{
enum CastMode
{
    LOCAL_MODE,
    LAN_MODE,
};
class DdsParticipant
{
public:
    DdsParticipant(){};
    virtual ~DdsParticipant()
    {
        eprosima::fastrtps::Domain::removeParticipant(mp_participant_);
    }

    /**
     * Implementation of participant creation. 
     * @param const std::string &name          Participant name, string.
     * @param int domain_id                    Domain Id. Ranges <0,232>.
     * @param CastMode mode                    Participant mode, default is LOCAL_MODE.
     *        LOCAL_MODE                       Messages would be only received locally via UDP unicast.
     *        LAN_MODE                         Messages would be multicasted via LAN.
     * @return Participant* ptr                Pointer to the created participant.
     * */

    eprosima::fastrtps::Participant *Create(const std::string &name, int domain_id, const CastMode mode = LOCAL_MODE)
    {
        eprosima::fastrtps::ParticipantAttributes PParam;
        PParam.rtps.builtin.domainId = domain_id;
        PParam.rtps.setName((const char *)(name.data()));
        if (mode == LOCAL_MODE)
        {
            auto customTransport = std::make_shared<eprosima::fastrtps::rtps::UDPv4TransportDescriptor>();
            (*customTransport).interfaceWhiteList.emplace_back("127.0.0.1");
            PParam.rtps.userTransports.push_back(customTransport);
            PParam.rtps.useBuiltinTransports = false;
        }
        mp_participant_ = eprosima::fastrtps::Domain::createParticipant(PParam);
        if (mp_participant_ == nullptr)
            return nullptr;

        return mp_participant_;
    }
    /**
     * Implementation of participant creation. 
     * @param string &xml_profile_name         Participant configuration xml file.
     * @return Participant* ptr                Pointer to the created participant.
     * */

    eprosima::fastrtps::Participant *Create(const std::string &xml_profile_name)
    {
        mp_participant_ = eprosima::fastrtps::Domain::createParticipant(xml_profile_name);
        if (mp_participant_ == nullptr)
            return nullptr;
        return mp_participant_;
    }
    eprosima::fastrtps::Participant *get_participant_ptr()
    {
        return mp_participant_;
    }

private:
    eprosima::fastrtps::Participant *mp_participant_;
};
};     // namespace cmdrDDS
#endif /* end of DDS_PARTICIPANT_H_*/
