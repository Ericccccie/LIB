### 无约束多元函数求极值

$$
1.求一元函数f(x)极值，解方程\\
f'(x)=0\\
2.二元函数f(x,y)极值，解方程组\\
f'_x(x, y)=\frac{\partial f(x, y)}{\partial x}=0\\
f'_y(x,y)=\frac{\partial f(x,y)}{\partial y}=0
$$

### 有约束多元函数求极值(拉格朗日乘子法)

$$
max\ U(X_1, X_2)\\
s.t.\quad P_1X_1+P_2X_2=I\\
$$

$$
步骤：\\
1.构造约束条件函数\ h(X_1, X_2)=P_1X_1+P_2X_2-I\\
2.构造拉格朗日函数\ L(X_1, X_2, \lambda) = U(X_1, X_2) - \lambda h(X_1, X_2)\\
3.对拉格朗日函数各变量求偏导，令偏导等于0，解方程组：\\
L'_{X_1}(X_1,X_2,\lambda)=\frac{\partial L(X_1,X_2,\lambda)}{\partial X_1}=\frac{\partial U(X_1, X_2)}{\partial X_1}-\lambda\frac{\partial h(X_1,X_2)}{\partial X_1}=0\\
L'_{X_2}(X_1,X_2,\lambda)=\frac{\partial L(X_1,X_2,\lambda)}{\partial X_2}=\frac{\partial U(X_1, X_2)}{\partial X_2}-\lambda\frac{\partial h(X_1,X_2)}{\partial X_2}=0\\
L'_{\lambda}(X_1,X_2,\lambda)=\frac{\partial L(X_1,X_2,\lambda)}{\partial\lambda}=-h(X_1,X_2)=0\\
求出偏导得（上述第三个式子在本问题推导中没有意义）：\\
(1)\quad L'_{X_1}(X_1,X_2,\lambda)=\frac{\partial U(X_1, X_2)}{\partial X_1}-\lambda P_1=0\\
(2)\quad L'_{X_2}(X_1,X_2,\lambda)=\frac{\partial U(X_1, X_2)}{\partial X_2}-\lambda P_2=0\\
将(3)\quad\frac{\partial U(X_1, X_2)}{\partial X_1}=MU_1，(4)\quad \frac{\partial U(X_1, X_2)}{\partial X_2}=MU_2分别代入(1)(2):\\
(5)\quad MU_1-\lambda P_1=0\\
(6)\quad MU_2-\lambda P_2=0\\
联立(5)(6)\quad \frac{MU_1}{P_1}=\frac{MU_2}{P_2}
$$





