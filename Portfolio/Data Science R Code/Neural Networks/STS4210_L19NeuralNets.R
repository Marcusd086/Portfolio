#################################################################
# Neural Net In Class Code |
#           Homework Below V
install.packages("neuralnet")
library(neuralnet)

df<- read.csv("dividendinfo.csv")

scaled.df <- as.data.frame(scale(df))

summary(scaled.df)

normdata <- function(x){
  return((x-min(x))/(max(x) - min(x)))
}

norm.df <- as.data.frame(lapply(df,normdata))
summary(norm.df)

set.seed(123)
inds <- sample(1:nrow(df), floor(.8*nrow(df)))
set.seed(NULL)

train<- norm.df[inds,]
test <- norm.df[-inds,]

set.seed(123)
rm<- neuralnet(dividend ~ fcfps + earnings_growth + de + mcap + current_ratio, data= train, hidden= c(2,1), linear.output=FALSE)
set.seed(NULL)

rm$result.matrix
plot(rm)

preds <- predict(rm,newdata = test, type = "response")
head(preds)
preds <- as.factor(round(preds))

table(test$dividend, preds)

acc <- sum((preds) == test$dividend)/nrow(test)
acc

lm_res <- glm(dividend ~., data=train, family="binomial")

lm_preds <- predict(lm_res, newdata = test, type = "response")

log_class <- round(lm_preds)

table(log_class, test$dividend)

accuracy_lm <- mean(log_class == test$dividend)
accuracy_lm

#################################################################
# Neural Net HW
#Question 1:
#It is necessary to scale or normalize the data prior to fitting the network to ensure the
#weight of variables is consistent.
df<- read.csv("nba_logreg.csv")
sum(is.na(df))
df <- na.omit(df)
normdata <- function(x){
  return((x-min(x))/(max(x) - min(x)))
}

norm.df <- as.data.frame(lapply(df[,-1],normdata))
summary(norm.df)

norm.df$Name <- df$Name

set.seed(987)
inds <- sample(1:nrow(df), floor(.75*nrow(df)))
set.seed(NULL)

train<- norm.df[inds,]
test <- norm.df[-inds,]

########################################################
#Generalized linear models
#Data science through computation and statistical reasoning
#TARGET_5Yrs ~ GP + MIN + PTS
set.seed(987)
rm<- neuralnet(TARGET_5Yrs ~ GP + MIN + PTS, data= train, hidden= c(2,1), linear.output=FALSE)
set.seed(NULL)

rm$result.matrix
plot(rm)

preds <- predict(rm,newdata = test, type = "response")
head(preds)
preds <- as.factor(round(preds))

table(test$TARGET_5Yrs, preds)

acc <- sum((preds) == test$TARGET_5Yrs)/nrow(test)
acc

#Accuracy = 0.7057057

########################################################
#Generalized linear models
#TARGET_5Yrs ~ FGM + X3P.Made + FTM
set.seed(987)
rm<- neuralnet(TARGET_5Yrs ~ FGM + X3P.Made + FTM, data= train, hidden= c(2,1), linear.output=FALSE)
set.seed(NULL)

rm$result.matrix
plot(rm)

preds <- predict(rm,newdata = test, type = "response")
head(preds)
preds <- as.factor(round(preds))

table(test$TARGET_5Yrs, preds)

acc <- sum((preds) == test$TARGET_5Yrs)/nrow(test)
acc

#Accuracy = 0.6936937

########################################################
#Generalized linear models
#TARGET_5Yrs ~ REB + AST + BLK
set.seed(987)
rm<- neuralnet(TARGET_5Yrs ~ REB + AST + BLK, data= train, hidden= c(2,1), linear.output=FALSE)
set.seed(NULL)

rm$result.matrix
plot(rm)

preds <- predict(rm,newdata = test, type = "response")
head(preds)
preds <- as.factor(round(preds))

table(test$TARGET_5Yrs, preds)

acc <- sum((preds) == test$TARGET_5Yrs)/nrow(test)
acc

#Accuracy = 0.6636637

######################################################
#Linear model
lm_res <- glm(TARGET_5Yrs ~ GP + MIN + PTS, data=train, family="binomial")

lm_preds <- predict(lm_res, newdata = test, type = "response")

log_class <- round(lm_preds)

table(log_class, test$TARGET_5Yrs)

accuracy_lm <- mean(log_class == test$TARGET_5Yrs)
accuracy_lm
#Accuracy = 0.7057057

