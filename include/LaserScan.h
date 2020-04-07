// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*! 
 * @file LaserScan.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _COMMANDER_ROBOT_MSG_LASERSCAN_H_
#define _COMMANDER_ROBOT_MSG_LASERSCAN_H_

// TODO Poner en el contexto.
#include "Time.h"

#include <stdint.h>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <bitset>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif
#else
#define eProsima_user_DllExport
#endif

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(LaserScan_SOURCE)
#define LaserScan_DllAPI __declspec( dllexport )
#else
#define LaserScan_DllAPI __declspec( dllimport )
#endif // LaserScan_SOURCE
#else
#define LaserScan_DllAPI
#endif
#else
#define LaserScan_DllAPI
#endif // _WIN32

namespace eprosima
{
    namespace fastcdr
    {
        class Cdr;
    }
}

namespace commander_robot_msg
{
    /*!
     * @brief This class represents the structure LaserScan defined by the user in the IDL file.
     * @ingroup LASERSCAN
     */
    class LaserScan
    {
    public:

        /*!
         * @brief Default constructor.
         */
        eProsima_user_DllExport LaserScan();

        /*!
         * @brief Default destructor.
         */
        eProsima_user_DllExport ~LaserScan();

        /*!
         * @brief Copy constructor.
         * @param x Reference to the object commander_robot_msg::LaserScan that will be copied.
         */
        eProsima_user_DllExport LaserScan(const LaserScan &x);

        /*!
         * @brief Move constructor.
         * @param x Reference to the object commander_robot_msg::LaserScan that will be copied.
         */
        eProsima_user_DllExport LaserScan(LaserScan &&x);

        /*!
         * @brief Copy assignment.
         * @param x Reference to the object commander_robot_msg::LaserScan that will be copied.
         */
        eProsima_user_DllExport LaserScan& operator=(const LaserScan &x);

        /*!
         * @brief Move assignment.
         * @param x Reference to the object commander_robot_msg::LaserScan that will be copied.
         */
        eProsima_user_DllExport LaserScan& operator=(LaserScan &&x);

        /*!
         * @brief This function copies the value in member timestamp
         * @param _timestamp New value to be copied in member timestamp
         */
        inline eProsima_user_DllExport void timestamp(const commander_robot_msg::Time &_timestamp)
        {
            m_timestamp = _timestamp;
        }

        /*!
         * @brief This function moves the value in member timestamp
         * @param _timestamp New value to be moved in member timestamp
         */
        inline eProsima_user_DllExport void timestamp(commander_robot_msg::Time &&_timestamp)
        {
            m_timestamp = std::move(_timestamp);
        }

        /*!
         * @brief This function returns a constant reference to member timestamp
         * @return Constant reference to member timestamp
         */
        inline eProsima_user_DllExport const commander_robot_msg::Time& timestamp() const
        {
            return m_timestamp;
        }

        /*!
         * @brief This function returns a reference to member timestamp
         * @return Reference to member timestamp
         */
        inline eProsima_user_DllExport commander_robot_msg::Time& timestamp()
        {
            return m_timestamp;
        }
        /*!
         * @brief This function copies the value in member frame_id
         * @param _frame_id New value to be copied in member frame_id
         */
        inline eProsima_user_DllExport void frame_id(const std::string &_frame_id)
        {
            m_frame_id = _frame_id;
        }

        /*!
         * @brief This function moves the value in member frame_id
         * @param _frame_id New value to be moved in member frame_id
         */
        inline eProsima_user_DllExport void frame_id(std::string &&_frame_id)
        {
            m_frame_id = std::move(_frame_id);
        }

        /*!
         * @brief This function returns a constant reference to member frame_id
         * @return Constant reference to member frame_id
         */
        inline eProsima_user_DllExport const std::string& frame_id() const
        {
            return m_frame_id;
        }

        /*!
         * @brief This function returns a reference to member frame_id
         * @return Reference to member frame_id
         */
        inline eProsima_user_DllExport std::string& frame_id()
        {
            return m_frame_id;
        }
        /*!
         * @brief This function sets a value in member angle_min
         * @param _angle_min New value for member angle_min
         */
        inline eProsima_user_DllExport void angle_min(float _angle_min)
        {
            m_angle_min = _angle_min;
        }

        /*!
         * @brief This function returns the value of member angle_min
         * @return Value of member angle_min
         */
        inline eProsima_user_DllExport float angle_min() const
        {
            return m_angle_min;
        }

        /*!
         * @brief This function returns a reference to member angle_min
         * @return Reference to member angle_min
         */
        inline eProsima_user_DllExport float& angle_min()
        {
            return m_angle_min;
        }
        /*!
         * @brief This function sets a value in member angle_max
         * @param _angle_max New value for member angle_max
         */
        inline eProsima_user_DllExport void angle_max(float _angle_max)
        {
            m_angle_max = _angle_max;
        }

        /*!
         * @brief This function returns the value of member angle_max
         * @return Value of member angle_max
         */
        inline eProsima_user_DllExport float angle_max() const
        {
            return m_angle_max;
        }

        /*!
         * @brief This function returns a reference to member angle_max
         * @return Reference to member angle_max
         */
        inline eProsima_user_DllExport float& angle_max()
        {
            return m_angle_max;
        }
        /*!
         * @brief This function sets a value in member angle_increment
         * @param _angle_increment New value for member angle_increment
         */
        inline eProsima_user_DllExport void angle_increment(float _angle_increment)
        {
            m_angle_increment = _angle_increment;
        }

        /*!
         * @brief This function returns the value of member angle_increment
         * @return Value of member angle_increment
         */
        inline eProsima_user_DllExport float angle_increment() const
        {
            return m_angle_increment;
        }

        /*!
         * @brief This function returns a reference to member angle_increment
         * @return Reference to member angle_increment
         */
        inline eProsima_user_DllExport float& angle_increment()
        {
            return m_angle_increment;
        }
        /*!
         * @brief This function sets a value in member time_increment
         * @param _time_increment New value for member time_increment
         */
        inline eProsima_user_DllExport void time_increment(float _time_increment)
        {
            m_time_increment = _time_increment;
        }

        /*!
         * @brief This function returns the value of member time_increment
         * @return Value of member time_increment
         */
        inline eProsima_user_DllExport float time_increment() const
        {
            return m_time_increment;
        }

        /*!
         * @brief This function returns a reference to member time_increment
         * @return Reference to member time_increment
         */
        inline eProsima_user_DllExport float& time_increment()
        {
            return m_time_increment;
        }
        /*!
         * @brief This function sets a value in member scan_time
         * @param _scan_time New value for member scan_time
         */
        inline eProsima_user_DllExport void scan_time(float _scan_time)
        {
            m_scan_time = _scan_time;
        }

        /*!
         * @brief This function returns the value of member scan_time
         * @return Value of member scan_time
         */
        inline eProsima_user_DllExport float scan_time() const
        {
            return m_scan_time;
        }

        /*!
         * @brief This function returns a reference to member scan_time
         * @return Reference to member scan_time
         */
        inline eProsima_user_DllExport float& scan_time()
        {
            return m_scan_time;
        }
        /*!
         * @brief This function sets a value in member range_min
         * @param _range_min New value for member range_min
         */
        inline eProsima_user_DllExport void range_min(float _range_min)
        {
            m_range_min = _range_min;
        }

        /*!
         * @brief This function returns the value of member range_min
         * @return Value of member range_min
         */
        inline eProsima_user_DllExport float range_min() const
        {
            return m_range_min;
        }

        /*!
         * @brief This function returns a reference to member range_min
         * @return Reference to member range_min
         */
        inline eProsima_user_DllExport float& range_min()
        {
            return m_range_min;
        }
        /*!
         * @brief This function sets a value in member range_max
         * @param _range_max New value for member range_max
         */
        inline eProsima_user_DllExport void range_max(float _range_max)
        {
            m_range_max = _range_max;
        }

        /*!
         * @brief This function returns the value of member range_max
         * @return Value of member range_max
         */
        inline eProsima_user_DllExport float range_max() const
        {
            return m_range_max;
        }

        /*!
         * @brief This function returns a reference to member range_max
         * @return Reference to member range_max
         */
        inline eProsima_user_DllExport float& range_max()
        {
            return m_range_max;
        }
        /*!
         * @brief This function copies the value in member ranges
         * @param _ranges New value to be copied in member ranges
         */
        inline eProsima_user_DllExport void ranges(const std::vector<float> &_ranges)
        {
            m_ranges = _ranges;
        }

        /*!
         * @brief This function moves the value in member ranges
         * @param _ranges New value to be moved in member ranges
         */
        inline eProsima_user_DllExport void ranges(std::vector<float> &&_ranges)
        {
            m_ranges = std::move(_ranges);
        }

        /*!
         * @brief This function returns a constant reference to member ranges
         * @return Constant reference to member ranges
         */
        inline eProsima_user_DllExport const std::vector<float>& ranges() const
        {
            return m_ranges;
        }

        /*!
         * @brief This function returns a reference to member ranges
         * @return Reference to member ranges
         */
        inline eProsima_user_DllExport std::vector<float>& ranges()
        {
            return m_ranges;
        }
        /*!
         * @brief This function copies the value in member intensities
         * @param _intensities New value to be copied in member intensities
         */
        inline eProsima_user_DllExport void intensities(const std::vector<float> &_intensities)
        {
            m_intensities = _intensities;
        }

        /*!
         * @brief This function moves the value in member intensities
         * @param _intensities New value to be moved in member intensities
         */
        inline eProsima_user_DllExport void intensities(std::vector<float> &&_intensities)
        {
            m_intensities = std::move(_intensities);
        }

        /*!
         * @brief This function returns a constant reference to member intensities
         * @return Constant reference to member intensities
         */
        inline eProsima_user_DllExport const std::vector<float>& intensities() const
        {
            return m_intensities;
        }

        /*!
         * @brief This function returns a reference to member intensities
         * @return Reference to member intensities
         */
        inline eProsima_user_DllExport std::vector<float>& intensities()
        {
            return m_intensities;
        }

        /*!
         * @brief This function returns the maximum serialized size of an object
         * depending on the buffer alignment.
         * @param current_alignment Buffer alignment.
         * @return Maximum serialized size.
         */
        eProsima_user_DllExport static size_t getMaxCdrSerializedSize(size_t current_alignment = 0);

        /*!
         * @brief This function returns the serialized size of a data depending on the buffer alignment.
         * @param data Data which is calculated its serialized size.
         * @param current_alignment Buffer alignment.
         * @return Serialized size.
         */
        eProsima_user_DllExport static size_t getCdrSerializedSize(const commander_robot_msg::LaserScan& data, size_t current_alignment = 0);


        /*!
         * @brief This function serializes an object using CDR serialization.
         * @param cdr CDR serialization object.
         */
        eProsima_user_DllExport void serialize(eprosima::fastcdr::Cdr &cdr) const;

        /*!
         * @brief This function deserializes an object using CDR serialization.
         * @param cdr CDR serialization object.
         */
        eProsima_user_DllExport void deserialize(eprosima::fastcdr::Cdr &cdr);



        /*!
         * @brief This function returns the maximum serialized size of the Key of an object
         * depending on the buffer alignment.
         * @param current_alignment Buffer alignment.
         * @return Maximum serialized size.
         */
        eProsima_user_DllExport static size_t getKeyMaxCdrSerializedSize(size_t current_alignment = 0);

        /*!
         * @brief This function tells you if the Key has been defined for this type
         */
        eProsima_user_DllExport static bool isKeyDefined();

        /*!
         * @brief This function serializes the key members of an object using CDR serialization.
         * @param cdr CDR serialization object.
         */
        eProsima_user_DllExport void serializeKey(eprosima::fastcdr::Cdr &cdr) const;

    private:
        commander_robot_msg::Time m_timestamp;
        std::string m_frame_id;
        float m_angle_min;
        float m_angle_max;
        float m_angle_increment;
        float m_time_increment;
        float m_scan_time;
        float m_range_min;
        float m_range_max;
        std::vector<float> m_ranges;
        std::vector<float> m_intensities;
    };
}

#endif // _COMMANDER_ROBOT_MSG_LASERSCAN_H_