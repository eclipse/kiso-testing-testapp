#ifndef TESTENVIRONMENT_HPP
#define TESTENVIRONMENT_HPP

namespace testenvironment
{
enum class AssertTypes
{
  ASSERT_TRUE = 0, //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_FALSE,  //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_EQ,  //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_NE,  //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_LT,  //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_LE,  //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_GT,  //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_GE,  //NOLINT (readability-identifier-naming): we want keep googletest style
  ASSERT_STREQ  //NOLINT (readability-identifier-naming): we want keep googletest style
};


} // namespace testenvironment
#endif // TESTENVIRONMENT_HPP
