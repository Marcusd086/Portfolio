#STS4210 Lab 1

df <- read.csv("Boston2.csv")

library(randomForest)
set.seed(321)
rf_res <- randomForest(df$medv~., data=df, ntree=500, mtry=6,importance=TRUE)
set.seed(NULL)

rf_res$importance
help(importance)

#c
sqrt(mean((df$medv - rf_res$predicted)^2))

#d
varImpPlot(rf_res)

#e
rf_res$importance

#f
rf_res$importance[ , 1]/rf_res$importanceSD

#i
set.seed(321)
rf_res <- randomForest(df$medv~., data=df, ntree=500,mtry=4,importance=TRUE)
set.seed(NULL)
rf_res$importance

#j
sqrt(mean((df$medv - rf_res$predicted)^2))

#l
set.seed(321)
rf_res <- randomForest(df$medv~., data=df, ntree=500,mtry=12,importance=TRUE)
set.seed(NULL)
rf_res$importance

#m
sqrt(mean((df$medv - rf_res$predicted)^2))

#PART 2: Comparing Random Forest to Other Methods

#a
set.seed(121)
train_ind <- sample(1:nrow(df), floor(0.75*nrow(df)))
set.seed(NULL)

Train <- df[train_ind, ]
Test <- df[-train_ind, ]

#b
set.seed(321)
rf_res <- randomForest(medv~., data=Train, ntree=500, mtry=4)
set.seed(NULL)

#c
split_preds <- predict(rf_res, newdata = Test)
sqrt(mean((Test$medv - split_preds)^2))

#d
set.seed(321)
rf_res <- randomForest(medv~., data=Train, ntree=500, mtry=12)
split_preds <- predict(rf_res, newdata = Test)
sqrt(mean((Test$medv - split_preds)^2))

#e
reg_lm <- lm(medv ~.,data=Train)
lm_preds <- predict(reg_lm, newdata= Test)

lm_mse <- sqrt(mean((Test$medv - lm_preds)^2))

lm_mse
