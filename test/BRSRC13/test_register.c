#include "tests.h"

#include "CORE/FW/register.h"

void test_register_BrRegistryFind() {
    br_registry reg;
    br_model m;
    br_model m2;
    br_registry_entry* e;
    reg.find_failed_hook = NULL;
    m.identifier = "test-identifier1";
    m2.identifier = "test-identifier2";

    BrRegistryNew(&reg);
    BrRegistryAdd(&reg, &m);
    BrRegistryAdd(&reg, &m2);
    TEST_ASSERT_EQUAL_INT(2, reg.count);
    e = (br_registry_entry*)reg.list.head;
    TEST_ASSERT_EQUAL_PTR(&m2, e->item);
    TEST_ASSERT_EQUAL_STRING("test-identifier2", e->item[1]);

    TEST_ASSERT_NOT_NULL(e->node.next);
    e = (br_registry_entry*)e->node.next;
    TEST_ASSERT_EQUAL_PTR(&m, e->item);

    TEST_ASSERT_NULL(BrRegistryFind(&reg, "not-found"));
    TEST_ASSERT_NOT_NULL(BrRegistryFind(&reg, "test-identifier2"));
    TEST_ASSERT_NOT_NULL(BrRegistryFind(&reg, "test-identifier1"));
}

void test_register_BrRegistryRemove() {
    br_registry reg;
    br_model m;
    br_model m2;
    br_model m3;
    br_registry_entry* e;
    reg.find_failed_hook = NULL;
    m.identifier = "test-identifier1";
    m2.identifier = "test-identifier2";

    BrRegistryNew(&reg);
    BrRegistryAdd(&reg, &m);
    BrRegistryAdd(&reg, &m2);

    TEST_ASSERT_EQUAL_INT(2, reg.count);

    // try removing non-existing item
    TEST_ASSERT_NULL(BrRegistryRemove(&reg, &m3));
    // remove first item
    TEST_ASSERT_EQUAL_PTR(&m, BrRegistryRemove(&reg, &m));
    TEST_ASSERT_EQUAL_INT(1, reg.count);
    e = (br_registry_entry*)reg.list.head;
    TEST_ASSERT_EQUAL_PTR(&m2, e->item);
    TEST_ASSERT_NULL(e->node.next->next);

    // now add it back, try remove last item
    BrRegistryAdd(&reg, &m);
    TEST_ASSERT_EQUAL_PTR(&m2, BrRegistryRemove(&reg, &m2));
    TEST_ASSERT_EQUAL_INT(1, reg.count);
    e = (br_registry_entry*)reg.list.head;
    TEST_ASSERT_EQUAL_PTR(&m, e->item);
    TEST_ASSERT_NULL(e->node.next->next);
}

void test_register_suite() {
    RUN_TEST(test_register_BrRegistryFind);
    RUN_TEST(test_register_BrRegistryRemove);
}