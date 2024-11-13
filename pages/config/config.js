import bluetooth from '../../utils/bluetooth'

Page({
  data: {
    config: {
      frequency: 433000000,
      txPower: 20,
      spreadingFactor: 7,
      signalBandwidth: 125000,
      codingRate: 5,
      wifi: {
        ssid: '',
        password: ''
      },
      nodeId: ''
    }
  },

  // 更新配置
  async updateConfig() {
    try {
      await bluetooth.sendConfig(this.data.config)
      wx.showToast({
        title: '配置已更新',
        icon: 'success'
      })
    } catch (error) {
      wx.showToast({
        title: '更新失败',
        icon: 'none'
      })
    }
  },

  // 输入框变化处理
  onInputChange(e) {
    const { field } = e.currentTarget.dataset
    const { value } = e.detail
    
    if (field.includes('.')) {
      const [obj, key] = field.split('.')
      this.setData({
        [`config.${obj}.${key}`]: value
      })
    } else {
      this.setData({
        [`config.${field}`]: value
      })
    }
  }
})