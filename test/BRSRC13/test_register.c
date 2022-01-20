#include "tests.h"

#include "CORE/FW/register.h"
#include <string.h>


typedef struct {
    br_uint_32 _reserved;
    char *identifier;
} test_element;

static void test_register_BrRegistryNew() {
    br_registry reg;
    br_registry *result;

    memset(&reg, 0, sizeof(reg));

    result = BrRegistryNew(&reg);
    TEST_ASSERT_EQUAL_PTR(&reg, result);
    TEST_ASSERT_EQUAL_INT(0, reg.count);
}

static void test_register_BrRegistryAdd_BrRegistryRemove() {
    br_registry reg;
    void *result;

    memset(&reg, 0, sizeof(reg));

    test_element item1 = { .identifier = "item1", };
    test_element item2 = { .identifier = "item2", };
    test_element item3 = { .identifier = "item3", };

    BrRegistryNew(&reg);

    result = BrRegistryAdd(&reg, &item1);
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    TEST_ASSERT_EQUAL_INT(1, reg.count);
    TEST_ASSERT_EQUAL_PTR(&item1, ((br_registry_entry*)reg.list.head)->item);

    result = BrRegistryAdd(&reg, &item2);
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    TEST_ASSERT_EQUAL_INT(2, reg.count);
    TEST_ASSERT_EQUAL_PTR(&item2, ((br_registry_entry*)reg.list.head)->item);

    result = BrRegistryAdd(&reg, &item3);
    TEST_ASSERT_EQUAL_PTR(&item3, result);
    TEST_ASSERT_EQUAL_INT(3, reg.count);
    TEST_ASSERT_EQUAL_PTR(&item3, ((br_registry_entry*)reg.list.head)->item);

    result = BrRegistryRemove(&reg, &item1);
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    TEST_ASSERT_EQUAL_INT(2, reg.count);
    TEST_ASSERT_EQUAL_PTR(&item3, ((br_registry_entry*)reg.list.head)->item);

    result = BrRegistryRemove(&reg, &item1);
    TEST_ASSERT_NULL(result);
    TEST_ASSERT_EQUAL_INT(2, reg.count);
    TEST_ASSERT_EQUAL_PTR(&item3, ((br_registry_entry*)reg.list.head)->item);

    result = BrRegistryRemove(&reg, &item3);
    TEST_ASSERT_EQUAL_PTR(&item3, result);
    TEST_ASSERT_EQUAL_INT(1, reg.count);
    TEST_ASSERT_EQUAL_PTR(&item2, ((br_registry_entry*)reg.list.head)->item);

    result = BrRegistryRemove(&reg, &item2);
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    result = BrRegistryRemove(&reg, &item2);
    TEST_ASSERT_NULL(result);
    TEST_ASSERT_EQUAL_INT(0, reg.count);
}

static void test_register_offsets() {
    // offset of identifier of structs that can be put in a registry,
    // **MUST** be accessible as the 2nd element in a char pointer array.
    TEST_ASSERT_EQUAL_INT(sizeof(void*), offsetof(test_element, identifier));

    TEST_ASSERT_EQUAL_INT(sizeof(void*), offsetof(br_device, identifier));
    TEST_ASSERT_EQUAL_INT(sizeof(void*), offsetof(br_device_pixelmap, pm_identifier));
    TEST_ASSERT_EQUAL_INT(sizeof(void*), offsetof(br_material, identifier));
    TEST_ASSERT_EQUAL_INT(sizeof(void*), offsetof(br_model, identifier));
    TEST_ASSERT_EQUAL_INT(sizeof(void*), offsetof(br_pixelmap, identifier));
}

static void *impl_test_find_failed_hook(char *pattern) {
    return pattern;
}

static void test_register_BrRegistryFind() {
    br_registry reg;
    void *result;
    char *itemTxt = "item";
    test_element item1 = { .identifier = "item1", };
    test_element item2 = { .identifier = "item2", };
    test_element item3 = { .identifier = "item3", };

    memset(&reg, 0, sizeof(reg));

    BrRegistryNew(&reg);

     result = BrRegistryFind(&reg, "item1");
     TEST_ASSERT_NULL(result);

     reg.find_failed_hook = impl_test_find_failed_hook;
     result = BrRegistryFind(&reg, itemTxt);
     TEST_ASSERT_EQUAL_PTR(itemTxt, result);
     reg.find_failed_hook = NULL;

    result = BrRegistryAdd(&reg, &item1);
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    result = BrRegistryFind(&reg, "item*");
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    result = BrRegistryFind(&reg, "item");
    TEST_ASSERT_NULL(result);

    reg.find_failed_hook = impl_test_find_failed_hook;
    result = BrRegistryFind(&reg, itemTxt);
    TEST_ASSERT_EQUAL_PTR(itemTxt, result);
    reg.find_failed_hook = NULL;

    result = BrRegistryAdd(&reg, &item2);
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    result = BrRegistryAdd(&reg, &item3);
    TEST_ASSERT_EQUAL_PTR(&item3, result);

    TEST_ASSERT_EQUAL_INT(3, reg.count);

    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    result = BrRegistryFind(&reg, "item2");
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    result = BrRegistryFind(&reg, "item3");
    TEST_ASSERT_EQUAL_PTR(&item3, result);
    result = BrRegistryFind(&reg, "item*");
    TEST_ASSERT_EQUAL_PTR(&item3, result);

    result = BrRegistryRemove(&reg, &item1);
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    TEST_ASSERT_EQUAL_INT(2, reg.count);

    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_NULL(result);
    result = BrRegistryFind(&reg, "item2");
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    result = BrRegistryFind(&reg, "item3");
    TEST_ASSERT_EQUAL_PTR(&item3, result);
    result = BrRegistryFind(&reg, "item*");
    TEST_ASSERT_EQUAL_PTR(&item3, result);

    result = BrRegistryRemove(&reg, &item2);
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    result = BrRegistryRemove(&reg, &item3);
    TEST_ASSERT_EQUAL_PTR(&item3, result);

    TEST_ASSERT_EQUAL_INT(0, reg.count);
}

static void test_register_BrRegistryClear() {
    br_registry reg;
    void *result;
    test_element item1 = { .identifier = "item1", };
    test_element item2 = { .identifier = "item2", };
    test_element item3 = { .identifier = "item3", };

    memset(&reg, 0, sizeof(reg));

    BrRegistryNew(&reg);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    result = BrRegistryClear(&reg);
    TEST_ASSERT_EQUAL_PTR(&reg, result);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    result = BrRegistryAdd(&reg, &item1);
    result = BrRegistryAdd(&reg, &item2);
    result = BrRegistryAdd(&reg, &item3);
    TEST_ASSERT_EQUAL_INT(3, reg.count);
    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    result = BrRegistryFind(&reg, "item2");
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    result = BrRegistryFind(&reg, "item3");
    TEST_ASSERT_EQUAL_PTR(&item3, result);

    result = BrRegistryClear(&reg);
    TEST_ASSERT_EQUAL_PTR(&reg, result);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_NULL(result);
    result = BrRegistryFind(&reg, "item2");
    TEST_ASSERT_NULL(result);
    result = BrRegistryFind(&reg, "item3");
    TEST_ASSERT_NULL(result);
}

static void test_register_BrRegistryAddMany() {
    br_registry reg;
    void *result;
    int nb;
    test_element item1 = { .identifier = "item1", };
    test_element item1b = { .identifier = "item1b", };
    test_element item2 = { .identifier = "item2", };
    test_element item3 = { .identifier = "item3", };
    test_element item4 = { .identifier = "item4", };
    test_element *itemsAdd[] = { &item1, &item2, &item3, &item1b, };

    memset(&reg, 0, sizeof(reg));

    BrRegistryNew(&reg);

    TEST_ASSERT_EQUAL_INT(0, reg.count);
    BrRegistryAddMany(&reg, (void**)&itemsAdd, BR_ASIZE(itemsAdd));
    TEST_ASSERT_EQUAL_INT(4, reg.count);
    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    result = BrRegistryFind(&reg, "item2");
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    result = BrRegistryFind(&reg, "item3");
    TEST_ASSERT_EQUAL_PTR(&item3, result);
    result = BrRegistryFind(&reg, "item4");
    TEST_ASSERT_NULL(result);
    result = BrRegistryFind(&reg, "item1b");
    TEST_ASSERT_EQUAL_PTR(&item1b, result);

    BrRegistryClear(&reg);
}

static void test_register_BrRegistryRemoveMany() {
    br_registry reg;
    void *result;
    int nb;
    test_element item1 = { .identifier = "item1", };
    test_element item2 = { .identifier = "item2", };
    test_element item3 = { .identifier = "item3", };
    test_element item4 = { .identifier = "item4", };
    test_element *itemsAdd[] = { &item1, &item2, &item3, };
    test_element *itemsRemove[] = { &item1, &item2, &item4, };

    memset(&reg, 0, sizeof(reg));

    BrRegistryNew(&reg);

    nb = BrRegistryRemoveMany(&reg, (void**)&itemsRemove, BR_ASIZE(itemsRemove));
    TEST_ASSERT_EQUAL(0, nb);

    TEST_ASSERT_EQUAL_INT(0, reg.count);
    BrRegistryAddMany(&reg, (void**)&itemsAdd, BR_ASIZE(itemsAdd));
    TEST_ASSERT_EQUAL_INT(3, reg.count);
    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_EQUAL_PTR(&item1, result);
    result = BrRegistryFind(&reg, "item2");
    TEST_ASSERT_EQUAL_PTR(&item2, result);
    result = BrRegistryFind(&reg, "item3");
    TEST_ASSERT_EQUAL_PTR(&item3, result);
    result = BrRegistryFind(&reg, "item4");
    TEST_ASSERT_NULL(result);

    nb = BrRegistryRemoveMany(&reg, (void**)&itemsRemove, BR_ASIZE(itemsRemove));
    TEST_ASSERT_EQUAL(2, nb);
    TEST_ASSERT_EQUAL_INT(1, reg.count);
    result = BrRegistryFind(&reg, "item1");
    TEST_ASSERT_NULL(result);
    result = BrRegistryFind(&reg, "item2");
    TEST_ASSERT_NULL(result);
    result = BrRegistryFind(&reg, "item3");
    TEST_ASSERT_EQUAL_PTR(&item3, result);
    result = BrRegistryFind(&reg, "item4");
    TEST_ASSERT_NULL(result);

    BrRegistryClear(&reg);

    nb = BrRegistryRemoveMany(&reg, (void**)&itemsRemove, BR_ASIZE(itemsRemove));
    TEST_ASSERT_EQUAL(0, nb);
}

static void test_register_BrRegistryFindMany() {
    br_registry reg;
    int result;
    int nb;
    test_element item1a = { .identifier = "item1a", };
    test_element item1b = { .identifier = "item1b", };
    test_element item2a = { .identifier = "item2a", };
    test_element item2b = { .identifier = "item2b", };
    test_element* itemBuffer[5];

    memset(&reg, 0, sizeof(reg));

    BrRegistryNew(&reg);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item1a", (void**)itemBuffer, BR_ASIZE(itemBuffer));
    TEST_ASSERT_EQUAL_INT(0, result);

    BrRegistryAdd(&reg, &item1a);
    BrRegistryAdd(&reg, &item1b);
    BrRegistryAdd(&reg, &item2a);
    BrRegistryAdd(&reg, &item2b);
    TEST_ASSERT_EQUAL_INT(4, reg.count);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item", (void**)itemBuffer, BR_ASIZE(itemBuffer));
    TEST_ASSERT_EQUAL_INT(0, result);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item1a", (void**)itemBuffer, BR_ASIZE(itemBuffer));
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_PTR(&item1a, itemBuffer[0]);
    TEST_ASSERT_NULL(itemBuffer[1]);
    TEST_ASSERT_NULL(itemBuffer[2]);
    TEST_ASSERT_NULL(itemBuffer[3]);
    TEST_ASSERT_NULL(itemBuffer[4]);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item1?", (void**)itemBuffer, BR_ASIZE(itemBuffer));
    TEST_ASSERT_EQUAL_INT(2, result);
    TEST_ASSERT_EQUAL_PTR(&item1b, itemBuffer[0]);
    TEST_ASSERT_EQUAL_PTR(&item1a, itemBuffer[1]);
    TEST_ASSERT_NULL(itemBuffer[2]);
    TEST_ASSERT_NULL(itemBuffer[3]);
    TEST_ASSERT_NULL(itemBuffer[4]);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item?a", (void**)itemBuffer, BR_ASIZE(itemBuffer));
    TEST_ASSERT_EQUAL_INT(2, result);
    TEST_ASSERT_EQUAL_PTR(&item2a, itemBuffer[0]);
    TEST_ASSERT_EQUAL_PTR(&item1a, itemBuffer[1]);
    TEST_ASSERT_NULL(itemBuffer[2]);
    TEST_ASSERT_NULL(itemBuffer[3]);
    TEST_ASSERT_NULL(itemBuffer[4]);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item*", (void**)itemBuffer, BR_ASIZE(itemBuffer));
    TEST_ASSERT_EQUAL_INT(4, result);
    TEST_ASSERT_EQUAL_PTR(&item2b, itemBuffer[0]);
    TEST_ASSERT_EQUAL_PTR(&item2a, itemBuffer[1]);
    TEST_ASSERT_EQUAL_PTR(&item1b, itemBuffer[2]);
    TEST_ASSERT_EQUAL_PTR(&item1a, itemBuffer[3]);
    TEST_ASSERT_NULL(itemBuffer[4]);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item*", (void**)itemBuffer, 2);
    TEST_ASSERT_EQUAL_INT(2, result);
    TEST_ASSERT_EQUAL_PTR(&item2b, itemBuffer[0]);
    TEST_ASSERT_EQUAL_PTR(&item2a, itemBuffer[1]);
    TEST_ASSERT_NULL(itemBuffer[2]);
    TEST_ASSERT_NULL(itemBuffer[3]);
    TEST_ASSERT_NULL(itemBuffer[4]);

    BrRegistryClear(&reg);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    memset(itemBuffer, 0, sizeof(itemBuffer));
    result = BrRegistryFindMany(&reg, "item", (void**)itemBuffer, BR_ASIZE(itemBuffer));
    TEST_ASSERT_EQUAL_INT(0, result);
}

static void test_register_BrRegistryCount() {
    br_registry reg;
    int result;
    int nb;
    test_element item1a = { .identifier = "item1a", };
    test_element item1b = { .identifier = "item1b", };
    test_element item2a = { .identifier = "item2a", };
    test_element item2b = { .identifier = "item2b", };

    memset(&reg, 0, sizeof(reg));

    BrRegistryNew(&reg);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    result = BrRegistryCount(&reg, "item1a");
    TEST_ASSERT_EQUAL_INT(0, result);

    BrRegistryAdd(&reg, &item1a);
    BrRegistryAdd(&reg, &item1b);
    BrRegistryAdd(&reg, &item2a);
    BrRegistryAdd(&reg, &item2b);
    TEST_ASSERT_EQUAL_INT(4, reg.count);

    result = BrRegistryCount(&reg, "item1a");
    TEST_ASSERT_EQUAL_INT(1, result);
    result = BrRegistryCount(&reg, "item?a");
    TEST_ASSERT_EQUAL_INT(2, result);
    result = BrRegistryCount(&reg, "item*");
    TEST_ASSERT_EQUAL_INT(4, result);
    result = BrRegistryCount(&reg, "item*b");
    TEST_ASSERT_EQUAL_INT(2, result);

    BrRegistryClear(&reg);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    result = BrRegistryCount(&reg, "item1a");
    TEST_ASSERT_EQUAL_INT(0, result);
}

typedef struct {
    test_element* buffer[5];
    int bufferSize;
    int bufferCapacity;

    int stopAfter;          // callback returns non-zero value when this value reaches 0
    test_element* stopIf;   // callback returns non-zero value this item equals item
} test_enum_cbfn_data;

static br_uint_32 impl_test_enum_cbfn(void* item, void* user) {
    test_enum_cbfn_data* userData = (test_enum_cbfn_data*)user;
    if (userData->bufferSize < userData->bufferCapacity) {
        userData->buffer[userData->bufferSize] = (test_element*)item;
        userData->bufferSize++;
    }
    userData->stopAfter--;
    if (userData->stopAfter == 0) {
        return 1;
    }
    if (item == (void*)userData->stopIf) {
        return 1;
    }
    return 0;
}

static void test_register_BrRegistryEnum() {
    br_registry reg;
    int result;
    test_enum_cbfn_data cbfn_data;
    test_element item1a = { .identifier = "item1a", };
    test_element item1b = { .identifier = "item1b", };
    test_element item2a = { .identifier = "item2a", };
    test_element item2b = { .identifier = "item2b", };

    memset(&reg, 0, sizeof(reg));

    BrRegistryNew(&reg);
    TEST_ASSERT_EQUAL_INT(0, reg.count);

    memset(&cbfn_data, 0, sizeof(cbfn_data));
    cbfn_data.bufferCapacity = 4;
    cbfn_data.stopAfter = 99;
    result = BrRegistryEnum(&reg, "item*", &impl_test_enum_cbfn, &cbfn_data);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(0, cbfn_data.bufferSize);

    memset(&cbfn_data, 0, sizeof(cbfn_data));
    result = BrRegistryEnum(&reg, "item*", &impl_test_enum_cbfn, &cbfn_data);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(0, cbfn_data.bufferSize);

    BrRegistryAdd(&reg, &item1a);
    BrRegistryAdd(&reg, &item1b);
    BrRegistryAdd(&reg, &item2a);
    BrRegistryAdd(&reg, &item2b);
    TEST_ASSERT_EQUAL_INT(4, reg.count);

    memset(&cbfn_data, 0, sizeof(cbfn_data));
    cbfn_data.bufferCapacity = 5;
    cbfn_data.stopAfter = 99;
    result = BrRegistryEnum(&reg, "item*", &impl_test_enum_cbfn, &cbfn_data);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(4, cbfn_data.bufferSize);

    memset(&cbfn_data, 0, sizeof(cbfn_data));
    cbfn_data.bufferCapacity = 2;
    cbfn_data.stopAfter = 99;
    result = BrRegistryEnum(&reg, "item*", &impl_test_enum_cbfn, &cbfn_data);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(2, cbfn_data.bufferSize);

    memset(&cbfn_data, 0, sizeof(cbfn_data));
    cbfn_data.bufferCapacity = 5;
    cbfn_data.stopAfter = 2;
    result = BrRegistryEnum(&reg, "item*", &impl_test_enum_cbfn, &cbfn_data);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(2, cbfn_data.bufferSize);

    memset(&cbfn_data, 0, sizeof(cbfn_data));
    cbfn_data.bufferCapacity = 5;
    cbfn_data.stopAfter = 99;
    cbfn_data.stopIf = &item2a;
    result = BrRegistryEnum(&reg, "item*", &impl_test_enum_cbfn, &cbfn_data);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(3, cbfn_data.bufferSize);

    BrRegistryClear(&reg);
    TEST_ASSERT_EQUAL_INT(0, reg.count);
}

void test_register_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_register_BrRegistryNew);
    RUN_TEST(test_register_BrRegistryAdd_BrRegistryRemove);
    RUN_TEST(test_register_offsets);
    RUN_TEST(test_register_BrRegistryFind);
    RUN_TEST(test_register_BrRegistryClear);
    RUN_TEST(test_register_BrRegistryAddMany);
    RUN_TEST(test_register_BrRegistryRemoveMany);
    RUN_TEST(test_register_BrRegistryFindMany);
    RUN_TEST(test_register_BrRegistryCount);
    RUN_TEST(test_register_BrRegistryEnum);
}
