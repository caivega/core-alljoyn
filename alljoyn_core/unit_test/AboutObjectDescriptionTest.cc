/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/


#include <alljoyn/AboutObjectDescription.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/version.h>

#include <alljoyn/MsgArg.h>
#include <gtest/gtest.h>
#include <qcc/String.h>
#include <qcc/Thread.h>

using namespace ajn;
using namespace qcc;

TEST(AboutObjectDescriptionTest, Add)
{
    QStatus status = ER_FAIL;
    AboutObjectDescription aod;

    status = aod.Add("/About/DeviceIcon", "org.alljoyn.Icon");
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    const char* interfaces2[] = { "org.alljoyn.test", "org.alljoyn.game", "org.alljoyn.mediaplayer" };
    status = aod.Add("/org/alljoyn/test", interfaces2, 3);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    EXPECT_TRUE(aod.HasInterface("/About/DeviceIcon", "org.alljoyn.Icon"));

    EXPECT_TRUE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.test"));
    EXPECT_TRUE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.game"));
    EXPECT_TRUE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.mediaplayer"));

    EXPECT_FALSE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.Icon"));

    EXPECT_FALSE(aod.HasInterface("/About/DeviceIcon", "org.alljoyn.test"));
    EXPECT_FALSE(aod.HasInterface("/About/DeviceIcon", "org.alljoyn.game"));
    EXPECT_FALSE(aod.HasInterface("/About/DeviceIcon", "org.alljoyn.mediaplayer"));

    EXPECT_TRUE(aod.HasInterface("org.alljoyn.Icon"));

    EXPECT_TRUE(aod.HasInterface("org.alljoyn.test"));
    EXPECT_TRUE(aod.HasInterface("org.alljoyn.game"));
    EXPECT_TRUE(aod.HasInterface("org.alljoyn.mediaplayer"));

    EXPECT_FALSE(aod.HasInterface("org.alljoyn.IAmNotReal"));

//    MsgArg arg;
//    status = aod.GetMsgArg(&arg);
//    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
//
//    printf("******************\n%s\n*****************\n", arg.ToString().c_str());
}

TEST(AboutObjectDescriptionTest, Remove)
{
    QStatus status = ER_FAIL;
    AboutObjectDescription aod;

    status = aod.Add("/About/DeviceIcon", "org.alljoyn.Icon");
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    const char* interfaces2[] = { "org.alljoyn.test", "org.alljoyn.game", "org.alljoyn.mediaplayer" };
    status = aod.Add("/org/alljoyn/test", interfaces2, 3);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    EXPECT_TRUE(aod.HasInterface("/About/DeviceIcon", "org.alljoyn.Icon"));

    EXPECT_TRUE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.test"));
    EXPECT_TRUE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.game"));
    EXPECT_TRUE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.mediaplayer"));

    const char* remove_interfaces[] = { "org.alljoyn.game", "org.alljoyn.mediaplayer" };
    status = aod.Remove("/org/alljoyn/test", remove_interfaces, 2);

    EXPECT_TRUE(aod.HasInterface("/About/DeviceIcon", "org.alljoyn.Icon"));

    EXPECT_TRUE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.test"));
    EXPECT_FALSE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.game"));
    EXPECT_FALSE(aod.HasInterface("/org/alljoyn/test", "org.alljoyn.mediaplayer"));

    //MsgArg arg;
    //status = aod.GetMsgArg(&arg);
    //EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    //printf("******************\n%s\n*****************\n", arg.ToString().c_str());
}

TEST(AboutObjectDescriptionTest, GetMsgArg)
{
    QStatus status = ER_FAIL;
    AboutObjectDescription aod;

    status = aod.Add("/About/DeviceIcon", "org.alljoyn.Icon");
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    const char* interfaces2[] = { "org.alljoyn.test", "org.alljoyn.game", "org.alljoyn.mediaplayer" };
    status = aod.Add("/org/alljoyn/test", interfaces2, 3);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    MsgArg arg;
    status = aod.GetMsgArg(&arg);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    MsgArg* structarg;
    size_t s_size;
    status = arg.Get("a(oas)", &s_size, &structarg);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    ASSERT_EQ(2u, s_size);

    char** object_path = new char*[s_size];
    size_t* number_itfs = new size_t[s_size];
    MsgArg** interfaces_arg = new MsgArg *[s_size];
    for (size_t i = 0; i < s_size; ++i) {
        status = structarg[i].Get("(oas)", &(object_path[i]), &number_itfs[i], &interfaces_arg[i]);
        EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    }

    EXPECT_STREQ("/About/DeviceIcon", object_path[0]);
    ASSERT_EQ(1u, number_itfs[0]);
    char* intf;
    interfaces_arg[0][0].Get("s", &intf);
    EXPECT_STREQ("org.alljoyn.Icon", intf);
    EXPECT_STREQ("/org/alljoyn/test", object_path[1]);
    ASSERT_EQ(3u, number_itfs[1]);

    // This test makes some assumptions about order that may not always be true
    // if we see failures that is a result of right values in the wrong order
    // then this test should be modified to account for that.
    interfaces_arg[1][0].Get("s", &intf);
    EXPECT_STREQ("org.alljoyn.game", intf);
    interfaces_arg[1][1].Get("s", &intf);
    EXPECT_STREQ("org.alljoyn.mediaplayer", intf);
    interfaces_arg[1][2].Get("s", &intf);
    EXPECT_STREQ("org.alljoyn.test", intf);

    delete [] object_path;
    delete [] number_itfs;
    delete [] interfaces_arg;
    //printf("******************\n%s\n*****************\n", arg.ToString().c_str());
}

TEST(AboutObjectDescriptionTest, Initilize)
{
    QStatus status = ER_FAIL;
    AboutObjectDescription aod;

    status = aod.Add("/About/DeviceIcon", "org.alljoyn.Icon");
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    const char* interfaces2[] = { "org.alljoyn.test", "org.alljoyn.game", "org.alljoyn.mediaplayer" };
    status = aod.Add("/org/alljoyn/test", interfaces2, 3);

    MsgArg arg;
    status = aod.GetMsgArg(&arg);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    AboutObjectDescription aod1;
    status = aod1.Initialize(arg);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    EXPECT_TRUE(aod1.HasInterface("/About/DeviceIcon", "org.alljoyn.Icon"));

    EXPECT_TRUE(aod1.HasInterface("/org/alljoyn/test", "org.alljoyn.test"));
    EXPECT_TRUE(aod1.HasInterface("/org/alljoyn/test", "org.alljoyn.game"));
    EXPECT_TRUE(aod1.HasInterface("/org/alljoyn/test", "org.alljoyn.mediaplayer"));

    EXPECT_FALSE(aod1.HasInterface("/org/alljoyn/test", "org.alljoyn.Icon"));

    EXPECT_FALSE(aod1.HasInterface("/About/DeviceIcon", "org.alljoyn.test"));
    EXPECT_FALSE(aod1.HasInterface("/About/DeviceIcon", "org.alljoyn.game"));
    EXPECT_FALSE(aod1.HasInterface("/About/DeviceIcon", "org.alljoyn.mediaplayer"));

    MsgArg arg1;
    status = aod1.GetMsgArg(&arg1);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    //printf("******************\n%s\n*****************\n", arg1.ToString().c_str());
}

class AboutObjectDescriptionTestBusObject1 : public BusObject {
  public:
    AboutObjectDescriptionTestBusObject1(BusAttachment& bus, const char* path)
        : BusObject(path) {
        AddInterface(*bus.GetInterface("test.about.objectdescription.interface1"), true);
    }
};

class AboutObjectDescriptionTestBusObject2 : public BusObject {
  public:
    AboutObjectDescriptionTestBusObject2(BusAttachment& bus, const char* path)
        : BusObject(path) {
        AddInterface(*bus.GetInterface("test.about.objectdescription.interface2"), true);
    }
};


TEST(AboutObjectDescriptionTest, PopulateAutomaticallyFromBusObject) {
    QStatus status = ER_FAIL;
    static const char* interface = "<interface name='test.about.objectdescription.interface1'>"
                                   "  <method name='Ping'>"
                                   "    <arg name='out_arg' type='s' direction='in' />"
                                   "    <arg name='return_arg' type='s' direction='out' />"
                                   "  </method>"
                                   "  <signal name='Chirp'>"
                                   "    <arg name='sound' type='s' />"
                                   "  </signal>"
                                   "  <property name='volume' type='i' access='readwrite'/>"
                                   "</interface>";
    BusAttachment bus("AboutObjectDescriptionTest");
    status = bus.CreateInterfacesFromXml(interface);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    AboutObjectDescriptionTestBusObject1 busObject1(bus, "/test/path1");
    status = bus.RegisterBusObject(busObject1);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    AboutObjectDescription aod;
    status = bus.GetAboutObjectDescription(aod);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    EXPECT_TRUE(aod.HasInterface("test.about.objectdescription.interface1"));
    EXPECT_TRUE(aod.HasPath("/test/path1"));
    EXPECT_TRUE(aod.HasInterface("/test/path1", "test.about.objectdescription.interface1"));
}

TEST(AboutObjectDescriptionTest, PopulateAutomaticallyFromMultipleBusObjects) {
    QStatus status = ER_FAIL;
    static const char* interface1 = "<interface name='test.about.objectdescription.interface1'>"
                                    "  <method name='Ping'>"
                                    "    <arg name='out_arg' type='s' direction='in' />"
                                    "    <arg name='return_arg' type='s' direction='out' />"
                                    "  </method>"
                                    "</interface>";
    static const char* interface2 = "<interface name='test.about.objectdescription.interface2'>"
                                    "  <method name='Ping'>"
                                    "    <arg name='out_arg' type='s' direction='in' />"
                                    "    <arg name='return_arg' type='s' direction='out' />"
                                    "  </method>"
                                    "</interface>";
    BusAttachment bus("AboutObjectDescriptionTest");
    status = bus.CreateInterfacesFromXml(interface1);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = bus.CreateInterfacesFromXml(interface2);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    AboutObjectDescriptionTestBusObject1 busObject1(bus, "/test/path1");
    AboutObjectDescriptionTestBusObject2 busObject2(bus, "/test/path2");
    status = bus.RegisterBusObject(busObject1);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = bus.RegisterBusObject(busObject2);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    AboutObjectDescription aod;
    status = bus.GetAboutObjectDescription(aod);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    EXPECT_TRUE(aod.HasInterface("test.about.objectdescription.interface1"));
    EXPECT_TRUE(aod.HasPath("/test/path1"));
    EXPECT_TRUE(aod.HasInterface("/test/path1", "test.about.objectdescription.interface1"));

    EXPECT_TRUE(aod.HasInterface("test.about.objectdescription.interface2"));
    EXPECT_TRUE(aod.HasPath("/test/path2"));
    EXPECT_TRUE(aod.HasInterface("/test/path2", "test.about.objectdescription.interface2"));
}

TEST(AboutObjectDescriptionTest, PopulateAutomaticallyRemoveBusObject) {
    QStatus status = ER_FAIL;
    static const char* interface1 = "<interface name='test.about.objectdescription.interface1'>"
                                    "  <method name='Ping'>"
                                    "    <arg name='out_arg' type='s' direction='in' />"
                                    "    <arg name='return_arg' type='s' direction='out' />"
                                    "  </method>"
                                    "</interface>";
    static const char* interface2 = "<interface name='test.about.objectdescription.interface2'>"
                                    "  <method name='Ping'>"
                                    "    <arg name='out_arg' type='s' direction='in' />"
                                    "    <arg name='return_arg' type='s' direction='out' />"
                                    "  </method>"
                                    "</interface>";
    BusAttachment bus("AboutObjectDescriptionTest");
    status = bus.CreateInterfacesFromXml(interface1);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = bus.CreateInterfacesFromXml(interface2);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    AboutObjectDescriptionTestBusObject1 busObject1(bus, "/test/path1");
    AboutObjectDescriptionTestBusObject2 busObject2(bus, "/test/path2");
    status = bus.RegisterBusObject(busObject1);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = bus.RegisterBusObject(busObject2);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    AboutObjectDescription aod;
    status = bus.GetAboutObjectDescription(aod);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    EXPECT_TRUE(aod.HasInterface("test.about.objectdescription.interface1"));
    EXPECT_TRUE(aod.HasPath("/test/path1"));
    EXPECT_TRUE(aod.HasInterface("/test/path1", "test.about.objectdescription.interface1"));

    EXPECT_TRUE(aod.HasInterface("test.about.objectdescription.interface2"));
    EXPECT_TRUE(aod.HasPath("/test/path2"));
    EXPECT_TRUE(aod.HasInterface("/test/path2", "test.about.objectdescription.interface2"));

    bus.UnregisterBusObject(busObject1);

    // TODO need to do more updates on AboutObjectDescription to make it so
    // calling GetAboutObjectDescription can reuse the same object.
    AboutObjectDescription aod2;
    status = bus.GetAboutObjectDescription(aod2);

    EXPECT_FALSE(aod2.HasInterface("test.about.objectdescription.interface1"));
    EXPECT_FALSE(aod2.HasPath("/test/path1"));
    EXPECT_FALSE(aod2.HasInterface("/test/path1", "test.about.objectdescription.interface1"));

    EXPECT_TRUE(aod2.HasInterface("test.about.objectdescription.interface2"));
    EXPECT_TRUE(aod2.HasPath("/test/path2"));
    EXPECT_TRUE(aod.HasInterface("/test/path2", "test.about.objectdescription.interface2"));
}
