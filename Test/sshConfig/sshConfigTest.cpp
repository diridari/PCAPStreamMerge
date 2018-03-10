//
// Created by basto on 9/8/17.
//

#include "../gtest/gtest.h"
#include <search.h>
#include <ReadConfig.h>
TEST(sshConfig,readEmptyConfig){
// Problem : where is the file located...
    // for me the exec file is in cmake-build dir
    string s = "../Test/sshConfig/test1.txt";
    ReadConfig * reader = new ReadConfig(&s);
    reader->open();
    ASSERT_FALSE(reader->hasNext());
    ASSERT_EQ(reader->getNextValid(), nullptr);
    ASSERT_FALSE(reader->hasNext());
}
// testHost TestUser testCommand
TEST(sshConfig, readConfig){
    string s = "../Test/sshConfig/test2.txt";
    ReadConfig * reader = new ReadConfig(&s);
    reader->open();
    ASSERT_TRUE(reader->hasNext());
    ASSERT_TRUE(reader->hasNext());
    ReadConfig::entry * e = reader->getNextValid();
    ASSERT_EQ(e->client,"testHost" );
    ASSERT_EQ(e->user,"TestUser" );
    ASSERT_EQ(e->execute,"testCommand" );
    ASSERT_FALSE(reader->hasNext());

}

// testHost TestUser testCommand with space
TEST(sshConfig, readConfigWithSpace){
    string s = "../Test/sshConfig/test3.txt";
    ReadConfig * reader = new ReadConfig(&s);
    reader->open();
    ASSERT_TRUE(reader->hasNext());
    ASSERT_TRUE(reader->hasNext());
    ReadConfig::entry * e = reader->getNextValid();
    ASSERT_EQ(e->client,"testHost" );
    ASSERT_EQ(e->user,"TestUser" );
    ASSERT_EQ(e->execute,"testCommand with space" );
    ASSERT_FALSE(reader->hasNext());

}
//8.8.8.8  user command
//127.0.0.1 me isCool
TEST(sshConfig, readConfigWithMultibleEntrys){
    string s = "../Test/sshConfig/test4.txt";
    ReadConfig * reader = new ReadConfig(&s);
    reader->open();
    ASSERT_TRUE(reader->hasNext());

    ReadConfig::entry * e = reader->getNextValid();
    ASSERT_EQ(e->client,"8.8.8.8" );
    ASSERT_EQ(e->user,"user" );
    ASSERT_EQ(e->execute,"command x" );
    ASSERT_TRUE(reader->hasNext());
    e = reader->getNextValid();
    ASSERT_EQ(e->client,"127.0.0.1" );
    ASSERT_EQ(e->user,"me" );
    ASSERT_EQ(e->execute,"isCool" );
    ASSERT_FALSE(reader->hasNext());
}

TEST(sshConfig, readInvalidConfig){
    string s = "../Test/sshConfig/test5.txt";
    ReadConfig * reader = new ReadConfig(&s);
    reader->open();
    ASSERT_TRUE(reader->hasNext());

    ReadConfig::entry * e = reader->getNextValid();
    ASSERT_EQ(e->client,"8.8.8.8" );
    ASSERT_EQ(e->user,"user" );
    ASSERT_EQ(e->execute,"command x" );
    ASSERT_TRUE(reader->hasNext());
    e = reader->getNextValid();
    ASSERT_EQ(e->client,"127.0.0.1" );
    ASSERT_EQ(e->user,"me" );
    ASSERT_EQ(e->execute,"isCool" );
    ASSERT_FALSE(reader->hasNext());
}

//call that as Setup
TEST(sshConfig, readRemoteSetup){
    string s = "../Test/sshConfig/test6.txt";
    ReadConfig * reader = new ReadConfig(&s);
    reader->open();
    ASSERT_TRUE(reader->hasNext());

    ReadConfig::entry * e = reader->getNextValid();
    ASSERT_EQ(e->client,"8.8.8.8" );
    ASSERT_EQ(e->user,"user" );
    ASSERT_EQ(e->execute,"command x " );
    ASSERT_EQ(e->setUp," call that as Setup");
    ASSERT_TRUE(reader->hasNext());
    e = reader->getNextValid();
    ASSERT_EQ(e->client,"127.0.0.1" );
    ASSERT_EQ(e->user,"me" );
    ASSERT_EQ(e->execute,"isCool" );
    ASSERT_FALSE(reader->hasNext());
}