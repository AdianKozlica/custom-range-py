Example:
```py
from range_module import RangeObject

for i in RangeObject(0, 10): # default value for step is 1
  print(i)

for i in RangeObject(1, 10, 2):
  print(i)
```

Build:
```
  python3 setup.py build
```
