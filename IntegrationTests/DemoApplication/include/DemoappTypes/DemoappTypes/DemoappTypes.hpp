#ifndef DEMOAPPTYPES_DEMOAPPTYPES_HPP
#define DEMOAPPTYPES_DEMOAPPTYPES_HPP

#include <vector>
#include <memory>
#include <string>
#include <array>

namespace demoapptypes
{
  constexpr uint8_t LANES = 8u;

  enum class Tier : uint8_t
  {
    TierA = 0,
    TierB
  };

  enum class Style : uint8_t
  {
    FREESTYLE = 0,
    BACKSTROKE,
    BUTTERFLY,
    BREASTSTROKE
  };

  enum class Laps : uint8_t
  {
    _50M = 0,
    _100M,
    _200M,
    _400M
  };

  struct Round
  {
    Laps laps;
    Style style;
  };

  struct Athlete
  {
  // public:
  //   Athlete(std::string const &l_name, float l_time) : 
  //   name(l_name), time(l_time) {}
  //   virtual ~Athlete() = default;

  //   float lapsTime() { return time; }
  //   uint16_t AddPoints(uint16_t p) { return p + points; }

  // private:
    const char* name;
    float time;
    uint16_t points;
  };

  class Race
  {

  public:
    Race(std::vector<Athlete> vec) : athletes(std::move(vec)){};

    void operator+=(Athlete athlete)
    {
      athletes.push_back(athlete);
    }

    const std::vector<Athlete>& getAthletes() const{ return athletes; }

    Round round;
  private:
    std::vector<Athlete> athletes;
    

    Race(const Race &) = delete;
    Race &operator=(Race) = delete;
  };

  // Race::Race(std::vector<Athlete> vec) : athletes(std::move(vec)){}


struct CompetetionMessageT
{
  Tier competerionTier;
  Round round;
  std::array<Athlete, LANES> todashboard;
};

} // demoapptypes

#endif // DEMOAPPTYPES_DEMOAPPTYPES_HPP
