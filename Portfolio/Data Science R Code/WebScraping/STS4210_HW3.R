library(dplyr)
library(tidyverse)
#install.packages("rvest")
library(rvest)
library(tm)
library(wordcloud)

#1a
url <- "https://www.nfl.com/stats/team-stats/offense/passing/2019/reg/all"
url2 <- read_html(url)


teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
  html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
  unique() %>% sort()

att <- html_nodes(url2, "td:nth-child(2)") %>% html_text() %>%
  as.numeric()

cmp <- html_nodes(url2, "td:nth-child(3)") %>% html_text() %>%
  as.numeric()

yds_att <- html_nodes(url2, "td:nth-child(5)") %>% html_text() %>%
  as.numeric()

pass_yds <- html_nodes(url2, "td:nth-child(6)") %>% html_text() %>%
  as.numeric()

td <- html_nodes(url2, "td:nth-child(7)") %>% html_text() %>%
  as.numeric()

int <- html_nodes(url2, "td:nth-child(8)") %>% html_text() %>%
  as.numeric()

sck <- html_nodes(url2, "td:nth-child(15)") %>% html_text() %>%
  as.numeric()

df <- as.data.frame(cbind(teams, att, cmp, yds_att, pass_yds, td, int, sck))

df = df %>% mutate_at(vars(-teams), as.numeric)
df

#1b

mean(df$pass_yds)
is.na(df$pass_yds)

#1c
names(df) <- c("Team", paste0(names(df), "_O")[-1])
df

##############################################################################
#2a
url <- "https://www.nfl.com/stats/team-stats/offense/rushing/2019/reg/all"
url2 <- read_html(url)


teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
  html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
  unique() %>% sort()

att <- html_nodes(url2, "td:nth-child(2)") %>% html_text()# %>%
 # as.numeric()

rush_yds <- html_nodes(url2, "td:nth-child(3)") %>% html_text() #%>%
  #as.numeric()

ypc <- html_nodes(url2, "td:nth-child(4)") %>% html_text() #%>%
  #as.numeric()

td <- html_nodes(url2, "td:nth-child(5)") %>% html_text() #%>%
 # as.numeric()

df2 <- as.data.frame(cbind(teams, att, rush_yds, ypc, td))
df2 = df2 %>% mutate_at(vars(-teams), as.numeric)

df
#2b
mean(df2$rush_yds)
#2c
names(df2) <- c("Team", paste0(names(df2), "_O")[-1])
df2


##############################################################################
#3a
url <- "https://www.nfl.com/stats/team-stats/defense/passing/2019/reg/all"
url2 <- read_html(url)


teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
  html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
  unique() %>% sort()

att <- html_nodes(url2, "td:nth-child(2)") %>% html_text()

cmp <- html_nodes(url2, "td:nth-child(3)") %>% html_text() 

yds_att <- html_nodes(url2, "td:nth-child(5)") %>% html_text() 

pass_yds <- html_nodes(url2, "td:nth-child(6)") %>% html_text() 

td <- html_nodes(url2, "td:nth-child(7)") %>% html_text() 

int <- html_nodes(url2, "td:nth-child(8)") %>% html_text()
  
sck <- html_nodes(url2, "td:nth-child(9)") %>% html_text() 

df3 <- as.data.frame(cbind(teams, att, cmp, yds_att, pass_yds, td, int, sck))
df3 = df3 %>% mutate_at(vars(-teams), as.numeric)

df3
#3b
mean(df3$pass_yds)
#3c
names(df3) <- c("Team", paste0(names(df3), "_D")[-1])
df3

##############################################################################
#4a
url <- "https://www.nfl.com/stats/team-stats/defense/rushing/2019/reg/all"
url2 <- read_html(url)


teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
  html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
  unique() %>% sort()

att <- html_nodes(url2, "td:nth-child(2)") %>% html_text()# %>%
# as.numeric()

rush_yds <- html_nodes(url2, "td:nth-child(3)") %>% html_text() #%>%
#as.numeric()

ypc <- html_nodes(url2, "td:nth-child(4)") %>% html_text() #%>%
#as.numeric()

td <- html_nodes(url2, "td:nth-child(5)") %>% html_text() #%>%
# as.numeric()

df4 <- as.data.frame(cbind(teams, att, rush_yds, ypc, td))
df4 = df4 %>% mutate_at(vars(-teams), as.numeric)

df4
#4b
mean(df4$rush_yds)
#4c
names(df4) <- c("Team", paste0(names(df4), "_D")[-1])
df4

##############################################################################
#5a
dff <- list(df, df2, df3, df4) %>%
  reduce(full_join, by = "Team", suffix = c(".pass", ".rush"))
dff
#5b
dff <- dff %>%
  mutate(td_diff = (td_O.pass + td_O.rush) - (td_D.pass + td_D.rush),
         yds_diff = (pass_yds_O + rush_yds_O) - (pass_yds_D + rush_yds_D))
dff
#5c
lm.mod <- lm(td_diff ~ yds_diff, data = dff)

print(1 - (sum((lm.mod$residuals)^2)/
             sum((dff$td_diff - mean(dff$td_diff))^2)))

##############################################################################
#7
years <- seq(2010, 2019) %>% paste0(., "/reg/all")
pos <- c("offense/", "defense/")
type <- c("passing/", "rushing/")

dff <- data.frame()

for(i in 1:length(years)){
  
  print(i)

#op
  url <- paste0("https://www.nfl.com/stats/team-stats/", pos[1], type[1], years[i])
  url2 <- read_html(url)
  
  teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
    html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
    unique() %>% sort()
  
  att <- html_nodes(url2, "td:nth-child(2)") %>% html_text() 
  
  cmp <- html_nodes(url2, "td:nth-child(3)") %>% html_text() 
  
  yds_att <- html_nodes(url2, "td:nth-child(5)") %>% html_text()
  
  pass_yds <- html_nodes(url2, "td:nth-child(6)") %>% html_text() 
  
  td <- html_nodes(url2, "td:nth-child(7)") %>% html_text() 
  
  int <- html_nodes(url2, "td:nth-child(8)") %>% html_text() 
  
  sck <- html_nodes(url2, "td:nth-child(15)") %>% html_text() 
  
  df <- as.data.frame(cbind(teams, att, cmp, yds_att, pass_yds, td, int, sck))
  
  df = df %>% mutate_at(vars(-teams), as.numeric)
  
  names(df) <- c("Team", paste0(names(df), "_O")[-1])
  
  
#or
  url <- paste0(base.url, pos[1], type[2], years[i])
  url2 <- read_html(url)
  
  teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
    html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
    unique() %>% sort()
  
  att <- html_nodes(url2, "td:nth-child(2)") %>% html_text()
  
  rush_yds <- html_nodes(url2, "td:nth-child(3)") %>% html_text()
  
  ypc <- html_nodes(url2, "td:nth-child(4)") %>% html_text() 
  
  td <- html_nodes(url2, "td:nth-child(5)") %>% html_text() 
  
  df2 <- as.data.frame(cbind(teams, att, rush_yds, ypc, td))
  df2 = df2 %>% mutate_at(vars(-teams), as.numeric)

  names(df2) <- c("Team", paste0(names(df2), "_O")[-1])
  
#dp
  url <- paste0(base.url, pos[2], type[1], years[i])
  url2 <- read_html(url)
  
  teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
    html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
    unique() %>% sort()
  
  att <- html_nodes(url2, "td:nth-child(2)") %>% html_text()
  
  cmp <- html_nodes(url2, "td:nth-child(3)") %>% html_text() 
  
  yds_att <- html_nodes(url2, "td:nth-child(5)") %>% html_text() 
  
  pass_yds <- html_nodes(url2, "td:nth-child(6)") %>% html_text() 
  
  td <- html_nodes(url2, "td:nth-child(7)") %>% html_text() 
  
  int <- html_nodes(url2, "td:nth-child(8)") %>% html_text()
  
  sck <- html_nodes(url2, "td:nth-child(9)") %>% html_text() 
  
  df3 <- as.data.frame(cbind(teams, att, cmp, yds_att, pass_yds, td, int, sck))
  df3 = df3 %>% mutate_at(vars(-teams), as.numeric)

  names(df3) <- c("Team", paste0(names(df3), "_D")[-1])
  
#dr
  url <- paste0(base.url, pos[2], type[2], years[i])
  url2 <- read_html(url)
  
  teams <- html_nodes(url2, ".sortinitialorder\\: div+ div") %>% 
    html_text() %>% gsub("\\n", "", .) %>% trimws() %>%
    unique() %>% sort()
  
  att <- html_nodes(url2, "td:nth-child(2)") %>% html_text()
  
  rush_yds <- html_nodes(url2, "td:nth-child(3)") %>% html_text() 
  
  ypc <- html_nodes(url2, "td:nth-child(4)") %>% html_text()
  
  td <- html_nodes(url2, "td:nth-child(5)") %>% html_text() 
  
  df4 <- as.data.frame(cbind(teams, att, rush_yds, ypc, td))
  df4 = df4 %>% mutate_at(vars(-teams), as.numeric)
  
  names(df4) <- c("Team", paste0(names(df4), "_D")[-1])
  
  dff <- rbind(dff,
                          list(df, df2, df3,df4) %>% 
                            reduce(full_join, by = "Team", suffix = c(".pass", ".rush")) %>%
                            mutate(year = as.factor(years[i] %>% gsub("\\D", "", .))))
}

write_csv(dff, "NFLTeamStats2010_2019.csv")

##############################################################################
#8
css <- read_rds("covidSpeechesScot.rds") 
remove <- VCorpus(VectorSource(css)) %>%
  tm_map(removeNumbers) %>%
  tm_map(removePunctuation) %>%
  tm_map(stripWhitespace) %>%
  tm_map(content_transformer(tolower)) %>%
  tm_map(removeWords, stopwords("english"))

tdm <- TermDocumentMatrix(remove)
m <- as.matrix(tdm)

list <- sort(rowSums(m), decreasing = T)
css.df <- data.frame(word = names(list), freq = list)

set.seed(1234)
wordcloud(words = css.df$word, freq = css.df$freq,
          min.freq = 1,
          max.words = 100,
          random.order = T,
          rot.per = 0.25,
          colors = brewer.pal(8, "Dark2"))


