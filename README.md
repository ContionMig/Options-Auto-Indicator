# Options-Auto-Indicator
[[My Website]](https://mitsuzi.xyz/)

# Screenshot
![ScreenShot](https://github.com/ContionMig/Options-Auto-Indicator/blob/main/docs/demo.png)
![ScreenShot2](https://github.com/ContionMig/Options-Auto-Indicator/blob/main/docs/output.jpg)
![ScreenShot3](https://github.com/ContionMig/Options-Auto-Indicator/blob/main/docs/time.jpg)

# Important
I coded this quite long ago just to get started on an auto trader. So it's pretty under developed and only uses one stragety which is the 3 SMA. Basically making trades when the Short and Long SMA crossover each other while using the Base SMA to see the trend. It's a pretty basic strategy and is used quite a bit for binary options. Since i have developed another auto trader which works more efficency and has more stragies wtih a proper GUI, i decided to release this first "prototype" i made for fun. 

# Settings.cpp
- `root_path`: The root folder used for prices and the saved json file
- `best_input`: The file that all the best inputs will be saved under
- `prices`: the folder with all the prices

- `trade_price`: How much you want to spend per trade
- `profit_margin`: Whats the broker's profit margin

- `min_trades`: mininum trades for inputs
- `min_whole_per`: minimum percentage for the inputs for third time
- `min_second_per`: minimum percentage for the inputs for second time
- `min_first_per`: minimum percentage for the inputs for first time

- `first_time_length`: first time length (eg 2 weeks)
- `second_time_length`: second time length (eg 1 month)
- `whole_time_length`: third time length (eg 2 months)

- `no_threads`: number of threads per price

# Features

### Multi Threading/Currency
It supports multithreading and it also allows you to do multiple currencies at once. The files that the bot will check for will be located, by default, under ```./data/prices```. Therefore, make sure to dump all the prices data you want into the folder before running the bot. Under the ```Settings.cpp/h``` you can set the number of threads per price. 

### Speed
There were millions of possible inputs, therefore speed was key. From the screenshot above, you can see that it can process one input in less than 0.1 seccond. This means, you are able to procress every possible inputs in about a couple of days. The bot will also not try the same input more than once, which allows us to generate random numbers instead of going through a nested for loop method. 

### Best Inputs
The bot will save all the best inputs into ```./data/best.json``` json file and will use these inputs when the bot runs for the first time. So if you like to stop the bot mid-way and continue, you can do so without losing all your progress. With this json file, you can actually setup a seperate bot which just trades using the data in the json file, so you don't have to touch anything related to APIs in this project rather, you can do it in python using libraries for websites like IQOptions or other brokers.

# Disclaimer
##### FOR EDUCATIONAL AND INFORMATIONAL PURPOSES ONLY.
The information provided in or through this repo is for educational and informational purposes only and solely as a self-help tool for your own use.

# Data
You can gather the price data for most popular currency here
- https://forexsb.com/historical-forex-data
