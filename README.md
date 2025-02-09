# Main Page

<a id="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/marshmll/pixelminer">
    <img src="https://raw.githubusercontent.com/marshmll/pixelminer/refs/heads/main/src/.pixelminer/Default/doxygen_icon.png" alt="Logo" width="150" height="auto">
  </a>

  <h3 align="center">PixelMiner</h3>

  <p align="center">
    A 2D Game Engine built with C++ and Simple And Fast Multimedia Library (SFML)
    <br />
    <a href="https://pixelminer-docs.vercel.app"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/marshmll/pixelminer">View Demo</a>
    •
    <a href="https://github.com/marshmll/pixelminer/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#configurating-and-running">Configurating and running</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#running-the-project">Running the project</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

PixelMiner is a 2D open world top-down game that allows the player to explore unique and procedurally generated worlds, with multiple options to explore. The project also allows a multiplayer LAN game to share the experience with friends.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

This project was built with:

* [![C++][C++]][C++-url]
* [![SFML][SFML]][SFML-url]
* [![minizip-ng][minizip-ng]][minizip-ng-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Configurating and running

This project has shell scripts to help building the project. **It is recommended to use a Debian-based distro** to run this project, although it is possible to build and use in Windows.

This manual covers only Debian-based GNU/Linux distros.

### Prerequisites

Install these dependencies in your machine.

### Running the project

1. Install all needed dependencies by running the ```./dependencies.sh``` script (works with APT only, if you have a different distro, install these dependencies manually).
2. Run ```./debug.sh``` or the ```./release.sh``` script, if you want either the Debug or the Release mode build.
3. Wait for the external libraries and game sources to build. It may take a while.
4. When the code is built, the shell script will automatically run the executable.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Top contributors:

<a href="https://github.com/marshmll/pixelminer/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=marshmll/pixelminer" alt="contrib.rocks image" />
</a>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Renan Andrade - [Instagram](https://instagram.com/ssh.renan) - renandasilvaoliveiraandrade@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [SFML](https://www.sfml-dev.org/)
* [minizip-ng](https://github.com/zlib-ng/minizip-ng)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/marshmll/pixelminer.svg?style=for-the-badge
[contributors-url]: https://github.com/marshmll/pixelminer/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/marshmll/pixelminer.svg?style=for-the-badge
[forks-url]: https://github.com/marshmll/pixelminer/network/members
[stars-shield]: https://img.shields.io/github/stars/marshmll/pixelminer.svg?style=for-the-badge
[stars-url]: https://github.com/marshmll/pixelminer/stargazers
[issues-shield]: https://img.shields.io/github/issues/marshmll/pixelminer.svg?style=for-the-badge
[issues-url]: https://github.com/marshmll/pixelminer/issues
[license-shield]: https://img.shields.io/github/license/marshmll/pixelminer.svg?style=for-the-badge
[license-url]: https://github.com/marshmll/pixelminer/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/renan-oliveira-andrade
[cover]: https://raw.githubusercontent.com/marshmll/pixelminer/main/images/screenshot.png
[C++]: https://img.shields.io/badge/-C++-333333?style=flat&logo=C%2B%2B
[C++-url]: https://en.cppreference.com/w/
[SFML]: https://img.shields.io/badge/-SFML-333333?style=flat&logo=sfml
[minizip-ng]: https://img.shields.io/badge/-minizip-333333?style=flat
[SFML-url]: https://www.sfml-dev.org/
[minizip-ng-url]: https://github.com/zlib-ng/minizip-ng
