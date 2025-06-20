#include "unity.h"

#include "connector-test.h"
#include "configuration.h"
#include "connector.h"

#include <errno.h>

void test_connector_init_is_correct() {

    const Configuration config = configuration_generate_default_config();
    errno                      = 0;

    Connector connector = {0};
    int       ret       = connector_init(&connector, &config);

    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(0, errno);

    TEST_ASSERT_NOT_EQUAL(0, connector.socket);
    int       type;
    socklen_t len = sizeof(type);
    TEST_ASSERT_NOT_EQUAL(
        -1, getsockopt(connector.socket, SOL_SOCKET, SO_TYPE, &type, &len));
}

void test_connector_destroy_is_correct() {
    const Configuration config = configuration_generate_default_config();
    errno                      = 0;
    Connector connector        = {0};
    int       ret              = connector_init(&connector, &config);

    ret = connector_destroy(&connector);

    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(0, errno);

    int       type;
    socklen_t len = sizeof(type);
    TEST_ASSERT_EQUAL(
        -1, getsockopt(connector.socket, SOL_SOCKET, SO_TYPE, &type, &len));
}
