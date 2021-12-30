/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "vibrator_if.h"
#include "vibrator_type.h"

using namespace testing::ext;

namespace {
    uint32_t g_duration = 1000;
    uint32_t g_noDuration = 0;
    uint32_t g_sleepTime = 2000;
    const char *g_timeSequence = "haptic.clock.timer";
    const char *g_builtIn = "haptic.default.effect";
    const char *g_arbitraryStr = "arbitraryString";
    const struct VibratorInterface *g_vibratorDev = nullptr;
}

class HdfVibratorTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfVibratorTest::SetUpTestCase()
{
    g_vibratorDev = NewVibratorInterfaceInstance();
}

void HdfVibratorTest::TearDownTestCase()
{
    if (g_vibratorDev != nullptr) {
        FreeVibratorInterfaceInstance();
        g_vibratorDev = nullptr;
    }
}

void HdfVibratorTest::SetUp()
{
}

void HdfVibratorTest::TearDown()
{
}

/**
  * @tc.name: CheckVibratorInstanceIsEmpty
  * @tc.desc: Creat a vibrator instance. The instance is not empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, CheckVibratorInstanceIsEmpty, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);
}

/**
  * @tc.name: PerformOneShotVibratorDuration001
  * @tc.desc: Controls this vibrator to perform a one-shot vibrator at a given duration.
    Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, PerformOneShotVibratorDuration001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->StartOnce(g_duration);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: PerformOneShotVibratorDuration002
  * @tc.desc: Controls this vibrator to perform a one-shot vibrator at 0 millisecond.
      Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, PerformOneShotVibratorDuration002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->StartOnce(g_noDuration);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect001
  * @tc.desc: Controls this Performing Time Series Vibrator Effects.
      Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, ExecuteVibratorEffect001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect002
  * @tc.desc: Controls this Performing built-in Vibrator Effects.
    Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, ExecuteVibratorEffect002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect003
  * @tc.desc: Controls this Perform a Empty vibrator effect.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, ExecuteVibratorEffect003, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(NULL);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect004
  * @tc.desc: Controls this Performing Time Series Vibrator Effects.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, ExecuteVibratorEffect004, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ABNORMAL);
    EXPECT_EQ(endRet, HDF_FAILURE);

    endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect005
  * @tc.desc: Controls this vibrator to stop the vibrator.
      Controls this Performing Time Series Vibrator Effects.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, ExecuteVibratorEffect005, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect006
  * @tc.desc: Controls this vibrator to stop the vibrator.
      Controls this Perform built-in Vibrator Effects.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, ExecuteVibratorEffect006, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect007
  * @tc.desc: Controls this Perform a one-shot vibrator with a arbitrary string.
    Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, ExecuteVibratorEffect007, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_arbitraryStr);
    EXPECT_EQ(startRet, HDF_FAILURE);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}
