# Shadow
![image.png](asset/shadow.png)

# Room #ae6b062f
![room.png](asset/room.png)

# Binary Volume Hierarchies
## Benchmark
- n spheres
- 3 lights

|    n | RAW (ms) | BVH (ms) | AntiAliasing (10) | MultiThread |
|-----:|---------:|---------:|------------------:|------------:|
|    8 |       36 |        8 |               118 |         433 |
|   64 |      181 |       15 |               179 |         444 |
| 1000 |     2523 |       57 |               533 |         490 |
| 8000 |    20192 |      150 |              1290 |         560 |
|  64K |          |      427 |              3658 |         816 |
|   1M |          |     2258 |                   |        2668 |
|   8M |          |     6685 |                   |        6746 |
