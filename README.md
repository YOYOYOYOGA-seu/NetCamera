# NetCamera
# NetCamera
# NetCamera
# NetCamera
   
2019/11/3
稳定版15帧JPEG传输
需要解决：高帧率花屏
         低时钟频率不传图
         整理代码格式
         RGB流和JPEG流整合（最好加上malloc）
         Task整理（图像储存区保护，如何实现协调DCMI采样和图传（自动帧率））
   
   
2019/11/4
稳定版30帧（480Mhz主频）
需要解决：整理代码格式
         RGB流删除和JPEG整合（拍照模式和视频流模式）（最好加上malloc）
         Task整理（图像储存区保护，如何实现协调DCMI采样和图传（自动帧率））   
         增加模拟的控制Task(50%)cpu占用率               ////////
待添加：  拍照功能（用psram作为存储，转移到tf卡）

2019/11/6       (branch develop photo)
稳定版30帧（480*360以上分辨率）
按键暂停、开始视频流（节能模式）
按键拍照（800*600），基本否定了用psram作为图像存储（需要用buff搬运，太耗CPU时间）
整理代码格式、注释
改变数据发送方式（由轮询变为帧完成发送事件Event）
需要解决：400MHz下图传（改变摄像头时钟输入等，节能）///////
         每次开始视频流需要通过SCCB初始化,期间必须挂起调度器（对实时任务影响致命）/////////
待添加： TF卡文件系统


2019/11/10       (branch develop_tf)
初步实现照片存储到TF卡（FatFs移植成功）
其中CubeMX 的 FatFs存在 Bug(syscall.c中信号量等待函数)
需要解决： 存储的照片最后一行存在花纹（应该是由于数据错误或者丢失）
          解决照片文件夹建立与命名问题
拓展任务： 丰富文件系统，争取实现串口的终端文件控制(usb_cdc与wifi)//////////


2019/11/12       (branch develop_tf)
添加TF卡中照片文件夹建立与命名问题
待添加： wifi传输
