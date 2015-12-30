extern int OWNER_SEARCH_ROI;			// 以遺留物為中心,建立一個搜尋owner的圓形ROI, 此參數為圓形ROI的半徑長度(pixel單位)
extern int GMM_LEARN_FRAME;				// 系統初始化,用多少張frame學習背景
extern int MAX_SFG;						// 遺留物最大閥值(pixel單位)
extern int MIN_SFG;						// 遺留物最小閥值(pixel單位)
extern int MAX_FG;						// 前景最大閥值(pixel單位)
extern int MIN_FG;						// 前景最小閥值(pixel單位)
extern int BUFFER_LENGTH;				// 暫存前景資訊的buffer
extern double GMM_LONG_LEARN_RATE;		// Long term model學習速度
extern double GMM_SHORT_LEARN_RATE;		// Short term model學習速度
extern double INPUT_RESIZE;				// 將輸入影像縮小的倍數, 可以加速演算法處裡