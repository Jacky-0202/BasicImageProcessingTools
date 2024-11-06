# 自定義的QGraphicsScene

## 目錄結構

```
HiPointView/
├── HiPointGraphicsView.h      // 自定義 QGraphicsView 類的頭文件
├── HiPointGraphicsView.cpp    // 自定義 QGraphicsView 類的實現文件
├── MainWindow.ui              // 主窗口的 UI 文件
├── MainWindow.cpp             // 主窗口的實現文件
└── MainWindow.h               // 主窗口的頭文件
```

## 概述

`HiPointGraphicsView` 是一個自定義的 `QGraphicsView` 類，用於顯示和操作影像。它擴展了標準的 `QGraphicsView`，增加了滾輪縮放（zoom in、zoom out）和滑鼠拖拽平移功能。該類在多點查看或分析影像的應用中非常有用。

### 主要功能

1. **滾輪縮放**：使用滾輪可以放大或縮小影像，並設有縮放等級限制以避免過度縮放。
2. **平移影像**：按住滑鼠左鍵拖動可以平移影像，便於快速查看不同區域。
3. **自適應中心**：滾輪縮放時，畫面會自動調整以保持游標位置為中心。



### 1. HiPointGraphicsView.h

#### 類定義

`HiPointGraphicsView` 繼承自 `QGraphicsView`，定義了放大縮小的限制參數與滾輪事件的處理方法。

```cpp
#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

class HiPointGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit HiPointGraphicsView(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    qreal scaleFactor = 1.15;      // 縮放係數
    qreal zoomLevel = 0;           // 當前縮放等級
    qreal zoomLevelMin = -10;      // 最小縮放等級
    qreal zoomLevelMax = 10;       // 最大縮放等級
};
```

#### 程式碼說明

- **scaleFactor**：放大或縮小的比例因子（預設為 1.15），在每次滾輪滾動時調整視圖縮放。
- **zoomLevel**：記錄當前的縮放等級，以確保縮放操作在範圍內。
- **zoomLevelMin/zoomLevelMax**：定義縮放的最小和最大等級，避免影像過度放大或縮小。

### 2. HiPointGraphicsView.cpp

#### 類實現

構造函數和滾輪事件處理方法 `wheelEvent` 提供了平滑的縮放控制和游標中心調整。

```cpp
#include "HiPointGraphicsView.h"

HiPointGraphicsView::HiPointGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setDragMode(QGraphicsView::ScrollHandDrag);

    setBackgroundBrush(QBrush(Qt::black));  // Grey RGB(128, 128, 128)
}

void HiPointGraphicsView::wheelEvent(QWheelEvent* event)
{
    qreal factor;
    if (event->angleDelta().y() > 0) {  // Scroll wheel up
        if (zoomLevel >= zoomLevelMax) return;  // Check the zoom limit
        factor = scaleFactor;
        zoomLevel++;
    }
    else {
        if (zoomLevel <= zoomLevelMin) return;
        factor = 1.0 / scaleFactor;
        zoomLevel--;
    }

    // 獲取當前滾輪位置的場景坐標
    QPointF cursorPos = mapToScene(event->position().toPoint());

    // 按照縮放因子縮放場景
    scale(factor, factor);

    // 調整視圖以保持游標位置不變
    QPointF newCursorPos = mapToScene(event->position().toPoint());
    QPointF delta = newCursorPos - cursorPos;
    translate(delta.x(), delta.y());
}
```

---

## 使用方法

### 1. 在 `MainWindow` 中加入自定義視圖

以下是將 `HiPointGraphicsView` 自定義視圖嵌入到主窗口 `MainWindow` 的步驟，並將影像加載到場景中進行顯示。

#### 步驟：

1. **將`HiPointGraphicsView.cpp` `HiPointGraphicsView.h`加到專案下** 

2. **在 Qt Designer 中設置 `graphicsView` 為自定義類**

   - 打開 `mainwindow.ui` 文件。
   - 將 `QGraphicsView` 元件從 Qt Designer 中拖入 UI，並命名為 `graphicsView`。
   - 右鍵單擊 `graphicsView`元件，選擇 `Promote to...`
     - 在 `Promoted class name` 欄位中輸入 `HiPointGraphicsView`。
     - 在 `Header file` 欄位中輸入 `HiPointGraphicsView.h`。
     - 點擊 `Add` 和 `Promote`。
   - 這樣將 `graphicsView` 元件提升為 `HiPointGraphicsView` 類型，方便在 UI 中直接使用自定義功能。

3. **初始化 `QGraphicsScene`**

   - 在 `MainWindow` 的頭文件 (`MainWindow.h`) 中宣告一個 `QGraphicsScene*` 指針。

   - 在構造函數中初始化場景並設定到 `graphicsView` 中。

     ```cpp
     // MainWindow.h
     #pragma once
     
     #include <QtWidgets/QMainWindow>
     #include "ui_MainWindow.h"
     #include <QGraphicsScene>
     
     class MainWindow : public QMainWindow
     {
         Q_OBJECT
     
     public:
         explicit MainWindow(QWidget *parent = nullptr);
         ~MainWindow();
     
     private:
         Ui::MainWindowClass ui;
         QGraphicsScene* m_scene;  // 宣告場景指針
     };
     ```

4. **設置場景並加載影像**

   - 在 `MainWindow.cpp` 中設置場景並加載影像至 `graphicsView`：

     ```cpp
     #include "MainWindow.h"
     #include <QGraphicsPixmapItem>
     
     MainWindow::MainWindow(QWidget *parent)
         : QMainWindow(parent),
           m_scene(new QGraphicsScene(this))  // 正確地初始化 m_scene
     {
         ui.setupUi(this);
     
         // 設置場景到自定義視圖中
         ui.graphicsView->setScene(m_scene);
     
         // 加載影像
         QString imagePath = "images/lena.jpg";
         QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap(imagePath));
         m_scene->addItem(pixmapItem);
     }
     
     MainWindow::~MainWindow()
     {
         delete m_scene;  // 刪除場景
     }
     ```
     
     >補充
     >
     >有個小觀念就是通常不會在標頭檔下面做賦值的動作，使用 `new` 在堆上分配記憶體，這種初始化必須在 `.cpp` 文件的構造函數中完成。因為 C++ 在標頭檔案中只允許成員變數的直接初始化，但不能進行動態分配。動態分配不僅需要在 `.cpp` 文件中完成，也可以在建構中明確控制其生命周期，防止重複分配或錯誤釋放記憶體。

---

## TO DO : 將自定義類直接改成 QT 本身的元件

...

