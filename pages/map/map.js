Page({
  data: {
    latitude: 23.099994,
    longitude: 113.324520,
    markers: []
  },

  onLoad() {
    // 获取当前位置
    wx.getLocation({
      type: 'gcj02',
      success: (res) => {
        this.setData({
          latitude: res.latitude,
          longitude: res.longitude
        })
      }
    })

    // 监听设备状态更新，更新节点位置
    const app = getApp()
    if (app.globalData.deviceStatus) {
      this.updateMarkers(app.globalData.deviceStatus)
    }
  },

  // 更新地图标记
  updateMarkers(status) {
    if (status.nodes) {
      const markers = status.nodes.map((node, index) => ({
        id: index,
        latitude: node.latitude,
        longitude: node.longitude,
        title: node.nodeId,
        callout: {
          content: node.nodeId,
          display: 'ALWAYS'
        }
      }))
      this.setData({ markers })
    }
  }
})