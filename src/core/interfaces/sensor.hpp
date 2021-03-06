#pragma once

#include <boost/system/error_code.hpp>

#include <memory>
#include <string>
#include <string_view>

namespace core::interfaces
{
class Sensor
{
  public:
    using ReadCallback = std::function<void(const boost::system::error_code& e,
                                            const std::string value)>;

    class Id
    {
      public:
        virtual ~Id() = default;

        // Should be unique among various overloads of SensorId
        // To ensure unique names suggested implementation is as follows
        // virtual std::string_view type() const override
        // {
        //     static const std::string_view id =
        //         typeid(decltype(*this)).name();
        //     return id;
        // }
        virtual std::string_view type() const = 0;

        // Should be unique among sensors of the same class
        virtual std::string_view str() const = 0;

        virtual bool operator<(const Id& other) const final
        {
            return std::make_tuple(type(), str()) <
                   std::make_tuple(other.type(), other.str());
        }
    };

    virtual ~Sensor() = default;

    virtual std::shared_ptr<const Id> id() const = 0;
    virtual void async_read(interfaces::Sensor::ReadCallback&& callback) = 0;
    virtual void
        registerForUpdates(interfaces::Sensor::ReadCallback&& callback) = 0;

    virtual bool operator<(const Sensor& other) const final
    {
        return *id() < *other.id();
    }
};
} // namespace core::interfaces
