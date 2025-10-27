#include <gtest/gtest.h>
#include <Debugging/Logging.h>

int main(int argc, char** argv)
{
	LOGGING_INIT_LOGS("TestLog.log");
	LOGGING_TOGGLE(true);

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return result;
}