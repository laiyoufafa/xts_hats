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

#include "utest_stream_operator_impl.h"
#include "stream_operator_callback.h"
#include "v1_0/istream_operator_callback.h"

#define SURFACE_ID (12345 + 666 + 2333)
const int CAMERA_BUFFER_QUEUE_IPC = 654320;

void StreamOperatorImplTest::SetUpTestCase(void)
{
    std::cout << "Camera::StreamOperatorImpl SetUpTestCase" << std::endl;
}

void StreamOperatorImplTest::TearDownTestCase(void)
{
    std::cout << "Camera::StreamOperatorImpl TearDownTestCase" << std::endl;
}

void StreamOperatorImplTest::SetUp(void)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);

    ret = GetCameraIds();
    ASSERT_EQ(true, ret);

    ret = GetCameraDevice();
    ASSERT_EQ(true, ret);

    ret = GetStreamOperator();
    ASSERT_EQ(true, ret);
}

void StreamOperatorImplTest::TearDown(void)
{
    std::cout << "Camera::StreamOperatorImpl TearDown.." << std::endl;
}

/**
  * @tc.name: IsStreamsSupported
  * @tc.desc: IsStreamsSupported, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(StreamOperatorImplTest, SUB_DriverSystem_CameraHdi_5160, TestSize.Level0)
{
    OperationMode mode = NORMAL;
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);
    StreamInfo streamInfo = {};
    streamInfo.width_ = 1280;
    streamInfo.height_ = 720;
    streamInfo.format_ = CAMERA_FORMAT_RGBA_8888;
    streamInfo.dataspace_ = 10;
    streamInfo.intent_ = PREVIEW;
    streamInfo.tunneledMode_ = 5;
    std::vector<StreamInfo> infos;
    infos.push_back(streamInfo);
    StreamSupportType pType;

    std::vector<uint8_t> setting;
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(modeSetting, setting);
    CamRetCode ret = (CamRetCode)streamOperator_->IsStreamsSupported(
        mode, setting, infos, pType);
    ASSERT_EQ(HDI::Camera::V1_0::DYNAMIC_SUPPORTED, pType);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
}

/**
  * @tc.name: IsStreamsSupported
  * @tc.desc: IsStreamsSupported,NOT_SUPPORTED success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(StreamOperatorImplTest, SUB_DriverSystem_CameraHdi_5200, TestSize.Level0)
{
    OperationMode mode = NORMAL;
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);
    StreamInfo streamInfo = {};
    streamInfo.width_ = 1280;
    streamInfo.height_ = 720;
    streamInfo.format_ = CAMERA_FORMAT_RGBA_8888;
    streamInfo.dataspace_ = 10;
    streamInfo.intent_ = CUSTOM;
    streamInfo.tunneledMode_ = 5;
    std::vector<StreamInfo> infos;
    infos.push_back(streamInfo);
    StreamSupportType pType;

    std::vector<uint8_t> setting;
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(modeSetting, setting);
    CamRetCode ret = (CamRetCode)streamOperator_->IsStreamsSupported(
        mode, setting, infos, pType);
    ASSERT_EQ(HDI::Camera::V1_0::NOT_SUPPORTED, pType);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
}

/**
  * @tc.name: IsStreamsSupported
  * @tc.desc: IsStreamsSupported,RE_CONFIGURED_REQUIRED success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(StreamOperatorImplTest, SUB_DriverSystem_CameraHdi_5210, TestSize.Level0)
{
    sptr<OHOS::IBufferProducer> producer;
    OperationMode operationMode = NORMAL;
    StreamSupportType supportType;
    std::vector<StreamInfo> streamInfos;
    StreamInfo streamInfo = {};
    streamInfo.streamId_ = 1005;
    streamInfo.width_ = 640;
    streamInfo.height_ = 480;
    streamInfo.format_ = PIXEL_FMT_RGBA_8888;
    streamInfo.dataspace_ = 8;
    streamInfo.intent_ = PREVIEW;
    std::shared_ptr<StreamConsumer> previewConsumer = std::make_shared<StreamConsumer>();
    producer = previewConsumer->CreateProducer([this](void* addr, uint32_t size) {
        SaveYUV("preview", addr, size);
    });
    streamInfo.bufferQueue_ = new BufferProducerSequenceable(producer);
    streamInfo.bufferQueue_->producer_->SetQueueSize(8);
    streamInfo.tunneledMode_ = 5;
    streamInfos.push_back(streamInfo);
    CamRetCode ret = (CamRetCode)streamOperator_->CreateStreams(streamInfos);
    std::cout << "streamOperator->CreateStreams = " << ret << std::endl;
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
    std::vector<uint8_t> ability;
    std::vector<std::string> cameraIds;
    ret = (CamRetCode)cameraHost_->GetCameraIds(cameraIds);
    std::string cameraId = cameraIds.front();
    ret = (CamRetCode)cameraHost_->GetCameraAbility(cameraId, ability);
    ret = (CamRetCode)streamOperator_->CommitStreams(NORMAL, ability);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    OperationMode mode = NORMAL;
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);
    StreamInfo streamInfo1 = {};
    streamInfo1.streamId_ = 1006;
    streamInfo1.width_ = 1280;
    streamInfo1.height_ = 720;
    streamInfo1.format_ = CAMERA_FORMAT_RGBA_8888;
    streamInfo1.dataspace_ = 10;
    streamInfo1.intent_ = PREVIEW;
    streamInfo1.tunneledMode_ = 5;
    std::vector<StreamInfo> infos;
    infos.push_back(streamInfo1);
    StreamSupportType pType;

    std::vector<uint8_t> setting;
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(modeSetting, setting);
    ret = (CamRetCode)streamOperator_->IsStreamsSupported(
        mode, setting, infos, pType);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
    ASSERT_EQ(HDI::Camera::V1_0::RE_CONFIGURED_REQUIRED, pType);

    std::vector<int> streamIds = {1005};
    ret = (CamRetCode)streamOperator_->ReleaseStreams(streamIds);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
}

/**
  * @tc.name: Capture
  * @tc.desc: Capture, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(StreamOperatorImplTest, SUB_DriverSystem_CameraHdi_5170, TestSize.Level0)
{
    sptr<OHOS::IBufferProducer> producer;
    OperationMode operationMode = NORMAL;
    StreamSupportType supportType;
    std::vector<StreamInfo> streamInfos;
    StreamInfo streamInfo = {};
    streamInfo.streamId_ = 1005;
    streamInfo.width_ = 640;
    streamInfo.height_ = 480;
    streamInfo.format_ = PIXEL_FMT_RGBA_8888;
    streamInfo.dataspace_ = 8;
    streamInfo.intent_ = PREVIEW;
    std::shared_ptr<StreamConsumer> previewConsumer = std::make_shared<StreamConsumer>();
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    producer = previewConsumer->CreateProducer([this](OHOS::SurfaceBuffer* buffer) {
        SaveYUV("preview", buffer->GetVirAddr(), buffer->GetSize());
    });
#else
    producer = previewConsumer->CreateProducer([this](void* addr, uint32_t size) {
        SaveYUV("preview", addr, size);
    });
#endif
    streamInfo.bufferQueue_ = new BufferProducerSequenceable(producer);
    streamInfo.bufferQueue_->producer_->SetQueueSize(8);
    streamInfo.tunneledMode_ = 5;
    streamInfos.push_back(streamInfo);

    CamRetCode ret = (CamRetCode)streamOperator_->CreateStreams(streamInfos);
    std::cout << "streamOperator->CreateStreams = " << ret << std::endl;
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    std::vector<std::string> cameraIds;
    ret = (CamRetCode)cameraHost_->GetCameraIds(cameraIds);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    std::vector<uint8_t> ability;
    std::string cameraId = cameraIds.front();
    ret = (CamRetCode)cameraHost_->GetCameraAbility(cameraId, ability);
    ret = (CamRetCode)streamOperator_->CommitStreams(NORMAL, ability);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    int captureId = 2001;
    CaptureInfo captureInfo = {};
    captureInfo.streamIds_ = {streamInfo.streamId_};
    captureInfo.captureSetting_ = ability;
    captureInfo.enableShutterCallback_ = false;
    ret = (CamRetCode)streamOperator_->Capture(captureId, captureInfo, true);
    std::cout << "streamOperator->Capture = " << ret << std::endl;
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
    sleep(1);

    ret = (CamRetCode)streamOperator_->CancelCapture(captureId);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    std::vector<int> streamIds = {1005};
    ret = (CamRetCode)streamOperator_->ReleaseStreams(streamIds);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
}

/**
  * @tc.name: CreateStreams
  * @tc.desc: CreateStreams, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(StreamOperatorImplTest, SUB_DriverSystem_CameraHdi_5180, TestSize.Level0)
{
    sptr<OHOS::IBufferProducer> producer;
    sptr<OHOS::IBufferProducer> producer_;
    OperationMode operationMode = NORMAL;
    StreamSupportType supportType;
    std::vector<StreamInfo> streamInfos;
    StreamInfo streamInfo = {};
    streamInfo.streamId_ = 1001;
    streamInfo.width_ = 720;
    streamInfo.height_ = 480;
    streamInfo.intent_ = PREVIEW;
    streamInfo.format_ = PIXEL_FMT_YCRCB_420_SP;
    streamInfo.dataspace_ = 8;
    StreamConsumer previewConsumer;
    streamInfo.tunneledMode_ = 5;
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    producer = previewConsumer.CreateProducer([this](OHOS::SurfaceBuffer* buffer) {
        SaveYUV("preview", buffer->GetVirAddr(), buffer->GetSize());
    });
#else
    producer = previewConsumer.CreateProducer([this](void* addr, uint32_t size) {
        SaveYUV("preview", addr, size);
    });
#endif
    streamInfo.bufferQueue_ = new BufferProducerSequenceable(producer);
    streamInfos.push_back(streamInfo);

    StreamInfo streamInfoSnapshot = {};
    streamInfoSnapshot.streamId_ = 1002;
    streamInfoSnapshot.width_ = 720;
    streamInfoSnapshot.height_ = 480;
    streamInfoSnapshot.format_ = PIXEL_FMT_YCRCB_420_SP;
    streamInfoSnapshot.dataspace_ = 8;
    streamInfoSnapshot.intent_ = STILL_CAPTURE;
    StreamConsumer snapshotConsumer;
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    producer_ = snapshotConsumer->CreateProducer([this](OHOS::SurfaceBuffer* buffer) {
        SaveYUV("preview", buffer->GetVirAddr(), buffer->GetSize());
    });
#else
    producer_ = snapshotConsumer.CreateProducer([this](void* addr, uint32_t size) {
        SaveYUV("preview", addr, size);
    });
#endif
    streamInfoSnapshot.bufferQueue_ = new BufferProducerSequenceable(producer_);
    streamInfoSnapshot.tunneledMode_ = 5;
    streamInfos.push_back(streamInfoSnapshot);

    CamRetCode ret = (CamRetCode)streamOperator_->CreateStreams(streamInfos);
    std::cout << "streamOperator->CreateStreams = " << ret << std::endl;
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    std::vector<int> streamIds = {1001, 1002};
    ret = (CamRetCode)streamOperator_->ReleaseStreams(streamIds);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
}
#ifdef CAMERA_BUILT_ON_OHOS_LITE
HWTEST_F(StreamOperatorImplTest, UTestAttachBufferQueue, TestSize.Level0)
{
    sptr<OHOS::IBufferProducer> producer;
    sptr<OHOS::IBufferProducer> producer_;
    OperationMode operationMode = NORMAL;
    StreamSupportType supportType;
    std::vector<StreamInfo> streamInfos;
    StreamInfo streamInfo = {};
    streamInfo.streamId_ = 1011;
    streamInfo.width_ = 720;
    streamInfo.height_ = 480;
    streamInfo.format_ = PIXEL_FMT_YCRCB_420_SP;
    streamInfo.dataspace_ = 8;
    streamInfo.intent_ = PREVIEW;
    StreamConsumer previewConsumer;
    producer = previewConsumer.CreateProducer([this](OHOS::SurfaceBuffer* buffer) {
        SaveYUV("preview", buffer->GetVirAddr(), buffer->GetSize());
    });
    streamInfo.bufferQueue_ = new BufferProducerSequenceable(producer);
    streamInfo.tunneledMode_ = 5;
    streamInfos.push_back(streamInfo);

    StreamInfo streamInfoSnapshot = {};
    streamInfoSnapshot.streamId_ = 1012;
    streamInfoSnapshot.width_ = 1920;
    streamInfoSnapshot.dataspace_ = 8;
    streamInfoSnapshot.height_ = 960;
    streamInfoSnapshot.intent_ = STILL_CAPTURE;
    streamInfoSnapshot.format_ = PIXEL_FMT_YCRCB_420_SP;
    StreamConsumer snapshotConsumer;
    producer_ = snapshotConsumer.CreateProducer([this](OHOS::SurfaceBuffer* buffer) {
        SaveYUV("preview", buffer->GetVirAddr(), buffer->GetSize());
    });
    streamInfoSnapshot.bufferQueue_ = new BufferProducerSequenceable(producer_);
    streamInfoSnapshot.tunneledMode_ = 5;
    streamInfos.push_back(streamInfoSnapshot);

    CamRetCode ret = (CamRetCode)streamOperator_->CreateStreams(streamInfos);
    std::cout << "streamOperator->CreateStreams = " << ret << std::endl;
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    StreamConsumer preview_consumer;
    OHOS::sptr<OHOS::IBufferProducer> producerTemp =
        preview_consumer.CreateProducer([this](OHOS::SurfaceBuffer* buffer) {
        SaveYUV("preview", buffer->GetVirAddr(), buffer->GetSize());
    });
    OHOS::sptr<BufferProducerSequenceable> bufferQueue = new BufferProducerSequenceable(producerTemp);
    ret = (CamRetCode)streamOperator_->AttachBufferQueue(streamInfo->streamId_, bufferQueue);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    ret = (CamRetCode)streamOperator_->DetachBufferQueue(streamInfo->streamId_);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    std::vector<int> streamIds = {1011, 1012};
    ret = (CamRetCode)streamOperator_->ReleaseStreams(streamIds);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
}
#else

/**
  * @tc.name: AttachBufferQueue
  * @tc.desc: AttachBufferQueue, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(StreamOperatorImplTest, SUB_DriverSystem_CameraHdi_5190, TestSize.Level0)
{
    sptr<OHOS::IBufferProducer> producer;
    sptr<OHOS::IBufferProducer> producer_;
    OperationMode operationMode = NORMAL;
    StreamSupportType supportType;
    std::vector<StreamInfo> streamInfos;
    StreamInfo streamInfo = {};
    streamInfo.streamId_ = 1011;
    streamInfo.width_ = 720;
    streamInfo.height_ = 480;
    streamInfo.format_ = PIXEL_FMT_YCRCB_420_SP;
    streamInfo.dataspace_ = 8;
    streamInfo.intent_ = PREVIEW;
    StreamConsumer previewConsumer;
    producer = previewConsumer.CreateProducer([this](void* addr, uint32_t size) {
        SaveYUV("preview", addr, size);
    });
    streamInfo.bufferQueue_ = new BufferProducerSequenceable(producer);
    streamInfo.tunneledMode_ = 5;
    streamInfos.push_back(streamInfo);

    StreamInfo streamInfoSnapshot = {};
    streamInfoSnapshot.streamId_ = 1012;
    streamInfoSnapshot.width_ = 1920;
    streamInfoSnapshot.dataspace_ = 8;
    streamInfoSnapshot.height_ = 960;
    streamInfoSnapshot.intent_ = STILL_CAPTURE;
    streamInfoSnapshot.format_ = PIXEL_FMT_YCRCB_420_SP;
    StreamConsumer snapshotConsumer;
    producer_ = snapshotConsumer.CreateProducer([this](void* addr, uint32_t size) {
        SaveYUV("preview", addr, size);
    });
    streamInfoSnapshot.bufferQueue_ = new BufferProducerSequenceable(producer_);
    streamInfoSnapshot.tunneledMode_ = 5;
    streamInfos.push_back(streamInfoSnapshot);

    CamRetCode ret = (CamRetCode)streamOperator_->CreateStreams(streamInfos);
    std::cout << "streamOperator->CreateStreams = " << ret << std::endl;
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    StreamConsumer preview_consumer;
    OHOS::sptr<OHOS::IBufferProducer> producerTemp =
        preview_consumer.CreateProducer([this](void* addr, uint32_t size) {
        SaveYUV("preview", addr, size);
    });
    OHOS::sptr<BufferProducerSequenceable> bufferQueue = new BufferProducerSequenceable(producerTemp);
    ret = (CamRetCode)streamOperator_->AttachBufferQueue(streamInfo.streamId_, bufferQueue);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    ret = (CamRetCode)streamOperator_->DetachBufferQueue(streamInfo.streamId_);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);

    std::vector<int> streamIds = {1011, 1012};
    ret = (CamRetCode)streamOperator_->ReleaseStreams(streamIds);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
}
#endif
