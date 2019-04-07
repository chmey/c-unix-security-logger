# usecl - Unix SECurity Logger

You are browsing the repository of a Unix Security Logging Tool.

It was designed as a project assignment for University class.


Modern implementations of systemd on Linux no longer use the files `/var/log/authlog` and `/var/log/syslog` for logging (auditory) information.

Instead a more centralized approach with the `journalctl` is used to query the systemd journal [[1]](https://www.freedesktop.org/software/systemd/man/journalctl.html).

So writing a security audit tools for modern systemd implementation required reading from the `sd-journal.h`-api [[2]](https://www.freedesktop.org/software/systemd/man/sd-journal.html). **For now this is the only method used by this program.**

Future work includes the reading from the actual log files.

Additionally I will try to make the program more sophisticated: optionally writing the ouput of the program to a file stream is my first priority.

If you are interested and want to contribute, please create a merge request of the particular branch: `sd-journal` or `log-files`.

**Special thanks** to @portante for his description [[3]](https://gist.github.com/portante/ff7fb429c6f973aab377f7bb77b0ffdb) of some bugs that might occur when querying the api and how to play around them.
