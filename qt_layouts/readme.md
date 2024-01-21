

## github ssh 22端口超时
- open ' ~/.ssh/config'
```
Host github.com
  Hostname ssh.github.com
  Port 443
```
- test 
```
ssh -T -p 443 git@ssh.github.com
```