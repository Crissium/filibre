# Description

This is a calibre-like film manager, only replacing the toolbar with a menubar. It stores info in XML and can output the collection inventory to HTML and CSV. There is also a built-in mpv player.

# Features

- In-place import, no copy needed.
- A calibre-like user experience, with poster grid view and a left navigation panel.
- Can export to XML, CSV, HTML inventory.
- Double click for a in-app preview with a built-in mpv player.

# Build

This software is built with Qt 6 qmake, using OpenGLWidget. It also uses [pugixml](https://pugixml.org/) for XML parsing and libmpv for built-in video playing functionality.

# Install

If you use openSUSE, you can get it from [OBS](https://build.opensuse.org/package/show/home:Crissium/filibre) or search for filibre in s.o.o.

Or you can build it yourself with qmake6.

We are planning to provide a Windows Installer as well.

# Contributing

The authors are first-year university students and we wrote it for a school assignment; we are very inexperienced, as can be judged from our cluttered code. So from a new feature to a total rewrite, any contribution is welcome!

# Similar project(s)

- [GCStar](https://gitlab.com/Kerenoc/GCstar) a generic collection-manager written in Perl
