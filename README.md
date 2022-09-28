Arduino Connector - 1.0.0
=============

# DESCRIPTION

</br>

This method facilitates the process of connecting to the Arduino through an available port.
<table>
    <tr>
        <td>Linux / Mac OS: /dev/ttyUSB*</td>
        <td>Windows: \\\\.\\COM*</td>
    </tr>
</table>
</br>
# ISSUE: How to use ? 
Step 1. Build

```bash
cmake -DCMAKE_BUILD_TYPE=Release -B ./build 
cd ./build
cmake --build . --clean-first
```

Step 2. Run Daemon

```bash
  ./aconnectord 
```
Step 3. Run Client 
<h6>
sent message to daemon with option -p (print) <message> 
</h6>

```bash
  ./aconnector -p "Hello, World!"
```
Enjoy!
