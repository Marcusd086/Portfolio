#Code for support vector machines lecture
#Necessary packages
#install.packages('caTools')
library(caTools)
#install.packages('e1071')
library(e1071)
#install.packages('Rfast')
library(Rfast)

#Reading in the dataset
dataset <- read.csv("FISH.csv")

#looking for the unique values of species
unique(dataset$Species)

#Making species a factor
dataset$Species = factor(dataset$Species, levels = c("Bream", "Roach", "Whitefish", "Parkki", "Perch", "Pike", "Smelt"))

#Creating a subset for necessary variables
dataset<-subset(dataset, select = c(Species,Weight,Height))

#Standardizing the data
dataset[,c(2,3)] <- scale(dataset[,c(2,3)])

#Testing Training Split
set.seed(123)
inds <- sample(1:nrow(dataset), floor(.75*nrow(dataset)))
set.seed(NULL)

training_set<- dataset[inds,]
test_set <- dataset[-inds,]

#Performing SVM
classifier = svm(formula = Species ~ .,
                 data = training_set,
                 type = 'C-classification',
                 kernel = 'linear')
help(svm)

classifier

#Obtaining predictions
y_pred = predict(classifier, newdata = test_set[-1])

#Creating confusion matrix
(cm = table(test_set[, 1], y_pred))
mean(test_set$Species == y_pred)

#Removing any NA's from the set
set = na.omit(training_set)

#Establishing a sequence for the height and weight variables 
X1 = seq(min(set[, 2]) - 1, max(set[, 2]) + 1, by = 0.01)
X2 = seq(min(set[, 3]) - 1, max(set[, 3]) + 1, by = 0.01)
grid_set = expand.grid(X1, X2)

#Establishing labels and creating initial plot of points
colnames(grid_set) = c('Weight', 'Height')
y_grid = predict(classifier, newdata = grid_set)
plot(set[, -1],
     main = 'SVM (Training set)',
     xlab = 'Weight', ylab = 'Height',
     xlim = range(X1), ylim = range(X2))

#Plotting vector lines
contour(X1, X2, matrix(as.numeric(y_grid), length(X1), length(X2)), add = TRUE)

#Colorizing those vector lines
points(grid_set, pch = '.', 
       col = ifelse(y_grid == "Smelt", 'coral1',ifelse(y_grid == "Pike",'aquamarine',
                                                       ifelse(y_grid == "Bream",'blue',ifelse(y_grid == "Roach",'green',
                                                                                              ifelse(y_grid == "Whitefish",'pink',ifelse(y_grid == "Smelt",'purple','yellow')))))))
#Plotting the points and colorizing them based on their classification
points(set[,2], set[,3], pch = 21, 
       bg = ifelse(set[, 1] == "Smelt", 'chocolate1',ifelse(set[, 1] == "Pike",'cyan3',
                                                            ifelse(set[, 1] == "Bream",'navy', ifelse(set[, 1] == "Roach",'green2',
                                                                                                      ifelse(set[, 1] == "Whitefish",'pink',ifelse(set[, 1] == "Smelt",'Black','yellow')))))))

#KNN for comparison-------------------------------------------------------------
library(FNN)

Train<- training_set
Test <- test_set

#Build models to pick k value
test_acc <- rep(NA, 50)
Train<- na.omit(Train)
Test<- na.omit(Test)

Train$Species = as.character(Train$Species)
Test$Species = as.character(Test$Species)


for (i in 2:50) {
  set.seed(123)
  knn_class <- FNN::knn(train=Train[,c(2:3)], test=Test[,c(2:3)], cl=Train$Species, k=i, prob=FALSE)
  set.seed(NULL)
  test_acc[i] <- mean(knn_class==Test$Species)
}
plot(test_acc, type="b", xlab="k")
which.max(test_acc)

#Pick k=5
set.seed(123)
knn_class <- FNN::knn(train=Train[,c(2:3)], test=Test[,c(2:3)], cl=Train$Species, k=5, prob=FALSE)
set.seed(NULL)
table(knn_class, Test$Species)
mean(knn_class==Test$Species)
