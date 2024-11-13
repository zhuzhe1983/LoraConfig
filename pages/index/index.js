import bluetooth from '../../utils/bluetooth'

Page({
  data: {
    isConnected: false,
    deviceStatus: null,
    scanning: false
  },

  onLoad() {
    this.initBluetooth()
    this.startLocationUpdate()
  },

  async initBluetooth() {
    const initialized = await bluetooth.init()
    if (initialized) {
      bluetooth.onDeviceStatusChange = (status) => {
        this.setData({
          deviceStatus: status,
          isConnected: true
        })
      }
    }
  },

  // 开始定位更新
  startLocationUpdate() {
    wx.startLocationUpdate({
      success: () => {
        wx.onLocationChange(async (res) => {
          if (this.data.isConnected) {
            try {
              await bluetooth.sendLocation(res.latitude, res.longitude)
            } catch (error) {
              console.error('发送位置信息失败:', error)
            }
          }
        })
      },
      fail: (error) => {
        console.error('开启位置更新失败:', error)
      }
    })
  },

  // ... (保持其他方法不变)

  async disconnect() {
    await bluetooth.disconnect()
    this.setData({
      isConnected: false,
      deviceStatus: null
    })
    wx.stopLocationUpdate()
  }
})