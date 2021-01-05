# Intern in Data Processing Department, Juhe Data, Suzhou, CHN

-	Primarily understood the applications of Python in data processing
-	Took responsibility for performing data mining and data cleansing with sklearn library (mainly removing outlier and nan values) and building a simplified bank credit-scoring model
- Due to company's confidential policies, sketchy plan & navie data only (no codes) 
# data mining schedule
---
## 1. 数据预处理

### 1.1 缺失值处理

	a)	直接删除含缺失值样本
	b)	根据样本之间相似性填补（简单可以以中位数替代）
	c)  根据已有样本，通过机器学习回归算法反向计算缺失值

### 1.2 异常值处理

	a) 逻辑判定（如年龄不可能为负值）
	b) 箱线图判定

### 1.3 数据归一化
	a) Max-Min 归一化
	b) Z-score

### 1.4 连续值处理

	a) 监督分箱： weight of evidence
	b) 非监督分箱： 等距/等深

## 2. 数据探索性分析（原始数据的分布情况等等）

	a) 直方图
	b) 散点图
	c) 箱线图


## 3 特征选择

	a) 相关系数矩阵及可视化
	b) 根据信息值（infomation value）筛选特征


## 4 模型开发和评估

	a) 数据集拆分
	b) 模型选择（Logistic, SVM ,Random Forest...）
	3) 模型检验PR和ROC图，F1值和AUC值，KS值,LIFT图。。。。


## 5 信用评分卡
