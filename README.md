## Alpha Vantage Tools
### Simple CLI tool for downloading, parsing, and getting insights from the [Alpha Vantage API](alphavantage.co).

#### Installation
There are a few dependencies:
+ GNU Scientific Library (libgsl) @ 2.5
+ jsoncpp @ 1.7.4
+ libcurl @ 7.64
+ GNU Make @ 4.2.1

With that taken care of, it's as simple as
```bash
make
sudo make install
```

#### Setting Environment Variables
Though you can set these variables using command-line flags, you might find it easier to use environment variables:

```bash
AV_API_KEY # is your Alpha Vantage API Key.
AV_DOWNLOAD_DIR # is where downloaded JSON files will live.
```

### Orientation
The command-line interface takes a positional command followed by some flags. Before we can get to the more interesting stuff, we need some data from the API:

```bash
# the -s and --symbol flags take the stock's symbol.

$ avtools download -s NFLX # get time series data for Netflix.
> success!
```

You'll now be able to find the data in AV\_DOWNLOAD\_DIR.

You can list the data you've downloaded with the "list" command:

```bash
$ avtools list
> UAL    2020-06-12
> LUV    2020-06-09
> LUV    2020-06-08 
> DAL    2020-06-08
cont'd ....
```
and you can clean the directory with "clean":

```bash
$ avtools clean -a # deletes everything, so you can start fresh
$ avtools clean -s UAL,LUV # deletes all data for the given symbol(s)
$ avtools clean -o # if more than one file exists for any given symbol, delete all but the most recent.
```

The -o and -s symbols can be combined to give more granular control.
