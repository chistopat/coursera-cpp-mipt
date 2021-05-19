#pragma once
#include <memory>

namespace RAII {
    template<typename Provider>
    class Booking {
    public:
      Booking(Provider* provider, int value)
        : provider_(provider)
        , value_(value) {
      }

      ~Booking() {
          if (provider_) {
              provider_->counter--;
          }
      }

      Booking(const Booking & other) = delete;
      Booking& operator=(const Booking& other) = delete;

      Booking(Booking&& other) {
          if (this != &other) {
              this->provider_ = other.provider_;
              this->value_ = other.value_;
              other.provider_ = nullptr;
              other.value_ = 0;
          }
      }

      Booking& operator=(Booking&& other) {
          if (this != &other) {
              this->provider_ = other.provider_;
              this->value_ = other.value_;
              other.provider_ = nullptr;
              other.value_ = 0;
          }
          return *this;
      }
    private:
      Provider* provider_;
      int value_;
    };
} // namespace RAII
