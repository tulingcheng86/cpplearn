# JoyRTC

You can use this tool to control some devices using WebRTC, but currently only unity

## Run

### Cloud

```
LISTEN=0.0.0.0:8080 go run ./...
```

Environment variables

| Variable Name | Description           | Default        |
| ------------- | --------------------- | -------------- |
| `LISTEN`      | Server Listen Address | `0.0.0.0:8080` |

### WebUI Demo

```
npm install
npm run dev
```

### Adapter: Unity

Unity Version: `2022.1.23f1`

NOTE: Don't use `unity.cn` !!! `unity.cn` version not compatible docker image!!!

Use Unity run

NOTE: Do not use safemode, The first run need auto install websocket-sharp

**Maybe manual install `websocket-sharp`**

```
pushd unity/Assets && nuget install && popd
```





**相机的视频流是通过使用Unity的WebRTC插件来进行传输的。**
 1、在 Start() 函数中，使用 CaptureStream()方法从相机中获取视频流，并将其存储在 videoStream 变量中：

```
if (videoStream == null)
{
    videoStream = cam.CaptureStream(1280, 720);
}

```

2、在 `AddTracks()` 函数中，遍历视频流的轨道，并将每个轨道添加到WebRTC的发送器列表中：

```
csharpCopy Codeforeach (var track in videoStream.GetTracks())
{
    Debug.Log("track: " + track);
    pcSenders.Add(_pc.AddTrack(track, videoStream));
}
```

在这段代码中，`_pc` 是一个 `RTCPeerConnection` 对象，`pcSenders` 是一个存储发送器的列表。



**要添加音频流传输，要实现音频流传输，你需要执行以下步骤：**

1. 获取音频流，可以使用Unity中的 `AudioListener` 或其他音频设备。
2. 类似于视频流传输，你需要创建一个 `MediaStream` 对象来存储音频流。
3. 然后使用 `AddTrack()` 方法将音频流的轨道添加到WebRTC的 `RTCPeerConnection` 对象中的发送器列表中，类似于视频流：

```
csharpCopy Codeforeach (var track in audioStream.GetTracks())
{
    pcSenders.Add(_pc.AddTrack(track, audioStream));
}
```

这样就可以将音频流添加到WebRTC连接中，并与远程端进行传输。



值得注意的是，使用WebRTC进行音频流传输需要额外的音频编解码器配置和信令交换步骤。



# 一、切换摄像头模式

切换摄像头是怎么实现的

切换摄像头是在代码的`Update()`方法中实现的。

**首先**，在`Start()`方法中获取了一个名为`ThirdPersonCamera`的脚本组件，并将其赋值给`cameraScript`变量。这个脚本用于控制摄像头的行为。

**然后**，在`Update()`方法中，通过检查`enableCameraModeToggle`变量的值来确定是否需要切换摄像头模式。如果`enableCameraModeToggle`为`true`，则调用`cameraScript`的`ToggleModifyValues()`方法来切换摄像头模式。

`ToggleModifyValues()`方法是`ThirdPersonCamera`脚本中的一个自定义方法，可以根据需要进行修改。它可能会在不同的摄像头模式之间切换，例如第一人称视角和第三人称视角。

因此，通过设置`enableCameraModeToggle`变量为`true`，可以触发摄像头模式的切换。这样，当通过DataChannel发送`{"type":"camera_mode_toggle"}`消息时，就可以切换摄像头模式。



# 二、使用摄像头捕获视频流。 将视频流添加到WebRTC连接中的轨道。

## 是怎么实现的 ？

## joyrtc.cs中



在代码中，实现了使用摄像头捕获视频流并将其添加到WebRTC连接中的轨道的功能。以下是相关的代码段：

1. **在`Start()`方法中，通过调用`CaptureStream()`方法来捕获摄像头的视频流，并将其存储在名为`videoStream`的`MediaStream`变量中。**

   ```
   if (videoStream == null)
   {
     videoStream = cam.CaptureStream(1280, 720);
   }
   ```

   **2.在`AddTracks()`方法中，遍历`videoStream`中的所有轨道，并将每个轨道通过`AddTrack()`方法添加到WebRTC连接的发送器中。**

```c++
private void AddTracks()
{
  foreach (var track in videoStream.GetTracks())
  {
    Debug.Log("track: " + track);
    pcSenders.Add(_pc.AddTrack(track, videoStream));
  }
}
```

**3.在`AsyncWebRTCCoroutine()`协程中的循环中，在WebRTC连接建立后，调用`AddTracks()`方法来将视频流的轨道添加到WebRTC连接中。**

```c++
foreach (var track in videoStream.GetTracks())
{
  Debug.Log("track: " + track);
  _pc.AddTrack(track, videoStream);
}
```

通过以上代码，摄像头获取到的视频流会被添加到WebRTC连接中的轨道中进行传输。这样就实现了将摄像头捕获的视频流添加到WebRTC连接中的功能。



## joyrtc.ts中

1、添加接收视频的轨道

```c++
	pc.addTransceiver("video", { direction: "recvonly" });
```

2、通过调用 `addTransceiver` 方法添加了一个接收视频的轨道。当远程对等端发送音频和视频流时，`RTCPeerConnection` 会自动处理并触发 `ontrack` 事件。

```
	pc.ontrack = (event) => {
		var el = document.createElement(event.track.kind as "video");
		el.srcObject = event.streams[0];
		el.autoplay = true;
		el.controls = true;
		el.muted = true;
		el.style.width = "inherit";
		el.style.height = "inherit";

		this.root.appendChild(el);
	};
```

这段代码执行以下操作：

1. 使用 `document.createElement` 方法创建一个 `<video>` 元素。
2. 将接收到的视频轨道（`event.track`）的 `kind` 属性作为类型断言（`as "video"`）传递给 `createElement` 方法，以确保创建的元素是 `<video>` 元素。
3. 将接收到的视频轨道所属的 `MediaStream` 对象（`event.streams[0]`）赋值给 `<video>` 元素的 `srcObject` 属性，以将视频流绑定到元素上。
4. 设置 `<video>` 元素的一些属性，例如 `autoplay`（自动播放视频）、`controls`（显示控制条）、`muted`（静音播放）等。
5. 设置 `<video>` 元素的样式，使其宽度和高度与父元素相同。
6. 将 `<video>` 元素添加到 `this.root`（Shadow DOM 中的根元素）中，以在页面上显示接收到的视频。
7. 

# 三、处理传输过来的虚拟摇杆数据控制物体

**1、在`joyrtc`脚本中，通过`dataChannel.OnMessage`事件处理接收到的DataChannel消息。在处理这些消息时，使用`JsonUtility.FromJson`方法将消息的JSON字符串转换为`MessageData`对象。**

```c#
   dataChannel.OnMessage = bytes => {
      string message = System.Text.Encoding.UTF8.GetString(bytes);
      MessageData messageData = JsonUtility.FromJson<MessageData>(message);
      float joystick1X = messageData.joystick1.x;
      float joystick1Y = messageData.joystick1.y;
      float joystick2X = messageData.joystick2.x;
      float joystick2Y = messageData.joystick2.y;
      Debug.Log(message);
```

在`MessageData`对象中，包含了两个`MyObject`类型的属性`joystick1`和`joystick2`，分别表示两个操纵杆的输入数据。在处理消息时，从`messageData`对象中获取操纵杆的x和y坐标。

**2、然后，根据操纵杆的输入来控制物体的移动和旋转。通过修改`cube`游戏对象的位置和旋转来实现。**

```c#
      // 将x和y应用到物体的移动
      // 旋转
      cube.transform.rotation *= Quaternion.Euler(0, joystick2X * 2f, 0);
      // 移动
      Vector3 forwardVector = cube.transform.forward;
      Vector3 rightVector = cube.transform.right;
      Vector3 verticalMovement = cube.transform.up * joystick2Y * 0.1f;
      Vector3 horizontalMovement = (forwardVector * joystick1Y + rightVector * joystick1X) * 0.1f;
      cube.transform.position += verticalMovement + horizontalMovement;
    };
```

具体地，根据`joystick2`的x坐标值来旋转`cube`对象，使用`Quaternion.Euler`方法构造一个绕Y轴旋转的四元数，乘以`cube.transform.rotation`来实现旋转。

根据`joystick1`的x和y坐标值来计算物体的垂直和水平移动向量，并将其应用到`cube`对象的位置上。

例如，将垂直移动向量`verticalMovement`和水平移动向量`horizontalMovement`相加，并将结果添加到`cube.transform.position`上，即可实现物体的移动。

**需要注意的是，在处理消息之前，需要进行一些预处理工作，例如判断操纵杆是否按下，以及切换摄像机模式等。这些预处理工作可以根据具体需求进行定制。**

**总之，通过解析JSON消息，提取操纵杆的输入数据，并根据输入数据来控制物体的移动和旋转，就可以实现处理接收到的DataChannel消息并操控物体的功能。**

![image-20231104143716300](C:\Users\BT7274\AppData\Roaming\Typora\typora-user-images\image-20231104143716300.png)