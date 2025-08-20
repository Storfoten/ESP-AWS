#include <unity.h>
#include "led_controller.h"

LEDController led(2);

void setUp(void) {
    // Setup code before each test
}

void tearDown(void) {
    // Cleanup code after each test
}

void test_led_initialization(void) {
    led.begin();
    TEST_ASSERT_FALSE(led.getState());
}

void test_led_turn_on(void) {
    led.turnOn();
    TEST_ASSERT_TRUE(led.getState());
}

void test_led_turn_off(void) {
    led.turnOff();
    TEST_ASSERT_FALSE(led.getState());
}

void test_led_toggle(void) {
    bool initialState = led.getState();
    led.toggle();
    TEST_ASSERT_NOT_EQUAL(initialState, led.getState());
}

void RUN_UNITY_TESTS() {
    UNITY_BEGIN();
    RUN_TEST(test_led_initialization);
    RUN_TEST(test_led_turn_on);
    RUN_TEST(test_led_turn_off);
    RUN_TEST(test_led_toggle);
    UNITY_END();
}

int main(int argc, char **argv) {
    RUN_UNITY_TESTS();
    return 0;
}