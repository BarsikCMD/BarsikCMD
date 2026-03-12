const { app, BrowserWindow, Menu } = require('electron')

app.whenReady().then(() => {
  Menu.setApplicationMenu(null)  // убирает menubar
  const win = new BrowserWindow({ width: 1100, height: 750 })
  win.loadFile('index.html')
})

app.on('window-all-closed', () => app.quit())