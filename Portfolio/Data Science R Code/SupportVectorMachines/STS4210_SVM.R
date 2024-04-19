library(caTools)
#install.packages('e1071')
library(e1071)
#install.packages('Rfast')
library(Rfast)

###############################################################################
#Learning Support Vector Machines
#Code found on Geeks for Geeks

dataset <- read.csv("social.csv")
dataset$Purchased = factor(dataset$Purchased, levels = c(0, 1))
dataset = dataset[3:5]

set.seed(123)
split = sample.split(dataset$Purchased, SplitRatio = 0.75)

training_set = subset(dataset, split == TRUE)
test_set = subset(dataset, split == FALSE)

training_set[-3] = scale(training_set[-3])
test_set[-3] = scale(test_set[-3])

classifier = svm(formula = Purchased ~ .,
                 data = training_set,
                 type = 'C-classification',
                 kernel = 'linear')
classifier

y_pred = predict(classifier, newdata = test_set[-3])

cm = table(test_set[, 3], y_pred)
cm

set <- training_set
X1 <- seq(min(set[, 1]) -1, max(set[, 1]) + 1, by = 0.01)
X2 <- seq(min(set[, 2]) -1, max(set[, 2]) + 1, by = 0.01)
grid_set <- expand.grid(X1, X2)
colnames(grid_set) <- c('Age', 'EstimatedSalary')
prob_set <- predict(classifier, type = 'response', newdata = grid_set)
y_grid <- ifelse(prob_set==0, 1, 0)
plot(set[, -3],
     main = 'SVM (Train)',
     xlab = 'Age',
     ylab = 'Estimated Salary',
     xlim = range(X1),
     ylim = range(X2)
)

contour(X1, X2, matrix(as.numeric(y_grid), length(X1), length(X2)), add = TRUE)

points(grid_set, pch = '.', col = ifelse(y_grid == 1, 'coral1', 'aquamarine'))

points(set, pch = 21, bg = ifelse(set[, 3] == 1, 'green4', 'red3'))


#####################################################################
dataset <- read.csv("FISH.csv")
unique(dataset$Species)
dataset$Species = factor(dataset$Species, levels = c("Bream", "Roach", "Whitefish", "Parkki", "Perch", "Pike", "Smelt"))
dataset<-subset(dataset, select = c(Species,Weight,Height))

set.seed(123)
split = sample.split(dataset$Species, SplitRatio = 0.75)

training_set = subset(dataset, split == TRUE)
test_set = subset(dataset, split == FALSE)

training_set[-1] = scale(training_set[-1])
test_set[-1] = scale(test_set[-1])

classifier = svm(formula = Species ~ .,
                 data = training_set,
                 type = 'C-classification',
                 kernel = 'linear')

classifier

y_pred = predict(classifier, newdata = test_set[-1])
(cm = table(test_set[, 1], y_pred))
mean(test_set$Species == y_pred)

classifier$tot.nSV

set = na.omit(training_set)
X1 = seq(min(set[, 2]) - 1, max(set[, 2]) + 1, by = 0.01)
X2 = seq(min(set[, 3]) - 1, max(set[, 3]) + 1, by = 0.01)
grid_set = expand.grid(X1, X2)
colnames(grid_set) = c('Weight', 'Height')
y_grid = predict(classifier, newdata = grid_set)
plot(set[, -1],
     main = 'SVM (Training set)',
     xlab = 'Weight', ylab = 'Height',
     xlim = range(X1), ylim = range(X2))

contour(X1, X2, matrix(as.numeric(y_grid), length(X1), length(X2)), add = TRUE)
#"Bream", "Roach", "Whitefish", "Parkki", "Perch", "Pike", "Smelt"
#points(grid_set, pch = '.', col = ifelse(y_grid == 1, 'coral1', 'aquamarine'))
points(grid_set, pch = '.', 
       col = ifelse(y_grid == "Smelt", 'coral1',ifelse(y_grid == "Pike",'aquamarine',
                           ifelse(y_grid == "Bream",'blue',ifelse(y_grid == "Roach",'green',
                                         ifelse(y_grid == "Whitefish",'pink',ifelse(y_grid == "Smelt",'purple','yellow')))))))

#points(set, pch = 21, bg = ifelse(set[, 1] == "Smelt", 'aquamarine4',ifelse(set[, 1] == "Pike",'brown2',ifelse(set[, 1] == "Bream",'darkorange3',ifelse(set[, 1] == "Roach",'darkred',ifelse(set[, 1] == "Whitefish",'blueviolet',ifelse(set[, 1] == "Smelt",'Black','chocolate4')))))))
points(set[,2], set[,3], pch = 21, 
       bg = ifelse(set[, 1] == "Smelt", 'chocolate1',ifelse(set[, 1] == "Pike",'cyan3',
                            ifelse(set[, 1] == "Bream",'navy', ifelse(set[, 1] == "Roach",'green2',
                                            ifelse(set[, 1] == "Whitefish",'pink',ifelse(set[, 1] == "Smelt",'Black','yellow')))))))
