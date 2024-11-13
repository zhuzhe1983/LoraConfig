const app = getApp()

class BluetoothManager {
  // ... (保持之前的代码不变)

  // 添加发送位置信息的方法
  async sendLocation(latitude, longitude) {
    const locationData = {
      type: 'location',
      latitude,
      longitude
    }
    await this.writeData(app.globalData.configCharId, JSON.stringify(locationData))
  }

  // ... (保持之前的代码不变)
}

export default new BluetoothManager()