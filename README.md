```diff
+ 29 july 2026
+ new build system seems to be working great for both x64 and arm64.
+ I'd like to understand if it's possible for me to codesign uxplay-windows before making a new release
```

# FREE AirPlay to your Windows PC
Free as both in "freedom" and "free beer"!

## Installation
Download the latest version of uxplay-windows from [**releases**](https://github.com/leapbtw/uxplay-windows/releases/latest).

After installing, control uxplay-windows from it's [tray icon](https://www.odu.edu/sites/default/files/documents/win10-system-tray.pdf)! Right-click it to start or stop AirPlay. \
You can also set it to run automatically when your PC starts

## WIKI (FAQ / Troubleshooting / Help)
Please take a look at the [Wiki](https://github.com/leapbtw/uxplay-windows/wiki), it should contain everything you're looking for

> [!IMPORTANT]
> *Why is Windows Defender complaining during installation?*
> 
> ![alt text](https://raw.githubusercontent.com/leapbtw/uxplay-windows/refs/heads/x64/stuff/defender.png "defender")
>
> Just click on `More info` and it will let you install. It complains because the executable is not signed. If you don't trust this software you can always build it yourself! See below.
>
> If prompted by Windows Firewall, please **allow** uxplay-windows to ensure it functions properly.

<br>
<details>
<summary><strong>Building</strong></summary>

*How do I build this software myself?*
 
Please see [BUILDING.md](./docs/BUILDING.md)
<br>
</details>

<details>
<summary><strong>Advanced configuration</strong></summary>

<br>

UxPlay arguments are read from `arguments.txt`.

Configuration precedence:

1. `%ProgramData%\uxplay-windows\arguments.txt`
2. `%APPDATA%\leapbtw\uxplay-windows\arguments.txt`
3. built-in default: `-n uxplay-windows -nh`

The machine-wide file takes precedence when it exists, allowing administrators
to enforce a shared configuration. When it is absent, each user can maintain
their own configuration under `%APPDATA%`.

Environment variables are expanded when the app starts. For example:

```text
-n %COMPUTERNAME% -nh
```

#### Running a second instance

To mirror two devices at once, start a second copy with `--instance N` (N = 1-9):

```text
uxplay-windows.exe --instance 2
```

Without the option (or with `--instance 1`) nothing changes. For N >= 2 everything is kept separate per number:

- `%ProgramData%\uxplay-windows-N\arguments.txt`, then `%APPDATA%\leapbtw\uxplay-windows-N\arguments.txt`
- default arguments: `-n uxplay-windows-N -nh -m 02:55:58:50:4C:0N` (own name and device ID)
- settings (registry), the Bluetooth status file and the autostart entry (`uxplay-windows-N`, registered with `--instance N`)
- logs in `%LOCALAPPDATA%\uxplay-windows-N\logs`
- Bluetooth discovery is off by default, because two beacons on one machine can conflict

To tell the copies apart, add `--label TEXT` and `--frame COLOUR`:

```text
uxplay-windows.exe --label "Teacher" --frame green
uxplay-windows.exe --instance 2 --label "Parent" --frame blue
```

- `--label` sets the AirPlay name (replacing `-n`) and the video window title
- `--frame` draws an even border around the video (`black`, `green`, `blue`, `red`, `yellow` or `white`)
- `--tab` grows the top edge into a tab and draws the label in it; `--icon PATH` adds a PNG (transparency is kept) before the label
- `--label-size N` sets the label height in video pixels (default 80); the tab grows with it
- the drawing is applied through `-vc`, so an explicit `-vc` in `arguments.txt` turns it off

</details>

<details>
<summary><strong>Local development (x64 and ARM64)</strong></summary>

<br>

After installing MSYS2, the complete local build is one PowerShell command:

```powershell
.\build.ps1 package -Architecture x64
.\build.ps1 package -Architecture arm64
```

Each command produces a verified portable ZIP and MSI under the corresponding
`out\<architecture>\artifacts` directory. See the [developer
guide](./docs/DEVELOPERS-GUIDE.md) for prerequisites and additional commands.

</details>

<details>
<summary><strong>TODO</strong></summary>

<br>

- make an update checker
- include uxplay.exe for debugging purposes
- ~~pass arguments from uxplay-windows.exe to uxplay~~ or maybe not

</details>

<details>
<summary><strong>Known Issues</strong></summary>

<br>

~~uxplay bugs out when waking PC from Sleep~~
~~you can fix this by killing uxplay-windows.exe and restarting Bonjour Service, and restarting uxplay.exe. Also restarting your PC might fix this.~~  \
Apparently moving from Bonjour PS to mDNSResponder fixed it? :)

- Sometimes just after connecting the iPhone to uxplay-windows, the stream appears but frozen. Reconnecting a second time fixes this. Probably an upstream issue maybe already fixed in the newer UxPlay version. I need to investigate further

</details>

## Reporting Issues
Please report issues related to the build system created with GitHub Actions in this repository. For issues related to other parts of this software, report them in their respective repositories.

## License
Please take a look at the [LICENSE](./docs/LICENSE.rtf).
