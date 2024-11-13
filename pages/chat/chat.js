import bluetooth from '../../utils/bluetooth'

Page({
  data: {
    messages: [],
    inputContent: '',
    receiver: 'ALL',
    receivers: ['ALL']  // 可用的接收者列表
  },

  onLoad() {
    // 从设备状态更新中获取可用节点列表
    bluetooth.onDeviceStatusChange = (status) => {
      if (status.nodes) {
        this.setData({
          receivers: ['ALL', ...status.nodes]
        })
      }
    }
  },

  // 发送消息
  async sendMessage() {
    if (!this.data.inputContent) return

    try {
      await bluetooth.sendMessage(
        this.data.receiver,
        this.data.inputContent
      )

      // 添加到消息列表
      const messages = this.data.messages
      messages.push({
        content: this.data.inputContent,
        receiver: this.data.receiver,
        timestamp: new Date().toLocaleTimeString()
      })

      this.setData({
        messages,
        inputContent: ''
      })
    } catch (error) {
      wx.showToast({
        title: '发送失败',
        icon: 'none'
      })
    }
  },

  // 选择接收者
  onReceiverChange(e) {
    this.setData({
      receiver: e.detail.value
    })
  },

  // 输入框内容变化
  onInputChange(e) {
    this.setData({
      inputContent: e.detail.value
    })
  }
})