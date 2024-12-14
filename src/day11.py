

def count_stones(n: int, depth: int, maxdepth: int, cache: dict[tuple[int, int], int]) -> int:
  d_left = maxdepth - depth 
  if (n, d_left) in cache:
      return cache[(n, d_left)];
  if (depth >= maxdepth):
    cache[(n, d_left)] = 1
    return 1 

  depth += 1
  if (n == 0): 
      res = count_stones(1, depth, maxdepth, cache)
      cache[(n, d_left)] = res;
      return res;
  digits = 1
  ncopy = n // 10
  mask = 1
  while (ncopy > 0):
    digits += 1
    ncopy //= 10

  if (digits % 2 == 0):
    for i in range(0, digits // 2):
      mask *= 10
    res = count_stones(n // mask, depth, maxdepth, cache) + count_stones(n % mask, depth, maxdepth, cache)
    cache[(n, d_left)] = res
    return res
  res = count_stones(2024 * n, depth, maxdepth, cache)
  cache[(n, d_left)] = res
  return res


numbers = []
with open("inputs/input11.txt", "r") as f:
    content = f.read();
    for n_str in content.split(" "):
        numbers.append(int(n_str.strip()))

print(numbers)

sumt1 = sum(map(lambda n: count_stones(n, 0, 25, {}), numbers))
print("Q1: The sum is", sumt1)

sumt2 = sum(map(lambda n: count_stones(n, 0, 75, {}), numbers))
print("Q2: The sum is", sumt2)
