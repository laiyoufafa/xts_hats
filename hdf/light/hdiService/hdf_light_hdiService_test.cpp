/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "v1_0/ilight_interface.h"
#include "light_type.h"
#include <hdf_log.h>

using namespace OHOS::HDI::Light::V1_0;
using namespace testing::ext;

namespace {
    constexpr uint32_t g_sleepTime = 3;
    constexpr int32_t g_minLightId = HDF_LIGHT_ID_BATTERY;
    constexpr int32_t g_maxLightId = HDF_LIGHT_ID_ATTENTION;
    constexpr int32_t g_onTime = 500;
    constexpr int32_t g_offTime = 500;
    sptr<ILightInterface> g_lightInterface = nullptr;

class HdfLightHdiServiceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfLightHdiServiceTest::SetUpTestCase()
{
    g_lightInterface = ILightInterface::Get();
}

void HdfLightHdiServiceTest::TearDownTestCase()
{
}

void HdfLightHdiServiceTest::SetUp()
{
}

void HdfLightHdiServiceTest::TearDown()
{
}

/**
  * @tc.name: CheckLightInstanceIsEmpty
  * @tc.desc: Creat a light instance. The instance is not empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, CheckLightInstanceIsEmpty, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);
}

/**
  * @tc.name: GetLightInfo001
  * @tc.desc: Get light info.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0020, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface->GetLightInfo(info);
    EXPECT_EQ(0, ret);
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);
    }
}

/**
  * @tc.name: TurnOnLight001
  * @tc.desc: TurnOnLight.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0030, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface->GetLightInfo(info);
    EXPECT_EQ(HDF_SUCCESS, ret);
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);

        HdfLightEffect effect;
        effect.lightColor.colorValue.rgbColor.r = 255;
        effect.lightColor.colorValue.rgbColor.g = 0;
        effect.lightColor.colorValue.rgbColor.b = 0;
        effect.flashEffect.flashMode = LIGHT_FLASH_NONE;
        int32_t ret = g_lightInterface->TurnOnLight(iter.lightId, effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalSleep(g_sleepTime);
        ret = g_lightInterface->TurnOffLight(iter.lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
  * @tc.name: TurnOnLight002
  * @tc.desc: TurnOnLight.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0040, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface->GetLightInfo(info);
    EXPECT_EQ(0, ret);
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);

        HdfLightEffect effect;
        effect.lightColor.colorValue.rgbColor.r = 255;
        effect.lightColor.colorValue.rgbColor.g = 0;
        effect.lightColor.colorValue.rgbColor.b = 0;
        effect.flashEffect.flashMode = LIGHT_FLASH_BUTT;
        int32_t ret = g_lightInterface->TurnOnLight(iter.lightId, effect);
        EXPECT_EQ(LIGHT_NOT_FLASH, ret);
    }
}

/**
  * @tc.name: TurnOnLight003
  * @tc.desc: TurnOnLight.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0050, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface->GetLightInfo(info);
    EXPECT_EQ(0, ret);
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);

        HdfLightEffect effect;
        effect.lightColor.colorValue.rgbColor.r = 255;
        effect.lightColor.colorValue.rgbColor.g = 0;
        effect.lightColor.colorValue.rgbColor.b = 0;
        effect.flashEffect.flashMode = LIGHT_FLASH_BLINK;
        effect.flashEffect.onTime = g_onTime;
        effect.flashEffect.offTime = g_offTime;
        int32_t ret = g_lightInterface->TurnOnLight(iter.lightId, effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalSleep(g_sleepTime);
        ret = g_lightInterface->TurnOffLight(iter.lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
  * @tc.name: TurnOnLight005
  * @tc.desc: TurnOnLight.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0170, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface->GetLightInfo(info);
    EXPECT_EQ(HDF_SUCCESS, ret);
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);

        HdfLightEffect effect;
        effect.lightColor.colorValue.rgbColor.r = 255;
        effect.lightColor.colorValue.rgbColor.g = 0;
        effect.lightColor.colorValue.rgbColor.b = 0;
        effect.flashEffect.flashMode = LIGHT_FLASH_GRADIENT;
        int32_t ret = g_lightInterface->TurnOnLight(iter.lightId, effect);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
        OsalSleep(g_sleepTime);
        ret = g_lightInterface->TurnOffLight(iter.lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
  * @tc.name: DisableLight001
  * @tc.desc: DisableLight.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0160, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    int32_t ret  = g_lightInterface->TurnOnLight(HDF_LIGHT_ID_BUTT, effect);
    EXPECT_EQ(LIGHT_NOT_SUPPORT, ret);
    ret  = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_BUTT);
    EXPECT_EQ(LIGHT_NOT_SUPPORT, ret);
}

/**
  * @tc.name: DisableLight002
  * @tc.desc: DisableLight.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0180, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    int32_t ret  = g_lightInterface->TurnOnLight(HDF_LIGHT_ID_NOTIFICATIONS, effect);
    EXPECT_EQ(LIGHT_NOT_SUPPORT, ret);
    ret  = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_NOTIFICATIONS);
    EXPECT_EQ(LIGHT_NOT_SUPPORT, ret);
}

/**
  * @tc.name: TurnOnMultiLights001
  * @tc.desc: DTurnOnMultiLights.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_DriverSystem_LightHdi_0190, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface-> GetLightInfo(info);
    EXPECT_EQ(0, ret);
    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);
        std::vector<HdfLightColor> lightColor;
        struct HdfLightColor light;
        light.colorValue.rgbColor.b = 0;
        lightColor.push_back(light);
        ret = g_lightInterface->TurnOnMultiLights(iter.lightId, lightColor);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}
}