##Alpha Vantage Tools
###Simple CLI tool for downloading, parsing, and getting insights from the Alpha Vantage API.

####Installation
There are a few dependencies:
+ GNU Scientific Library (libgsl) @ 2.5
+ jsoncpp @ 0.11
+ libcurl @ 7.64
+ GNU Make @ 4.2.1

Then, it's as simple as
```bash
make
sudo make install
```

####Setting Environment Variables
Though you can set these variable using command-line flags, you might find it easier to use environment variables:

```bash
AV_API_KEY # is your Alpha Vantage API Key.
AV_DOWNLOAD_DIR # is where downloaded JSON files will live.
```

