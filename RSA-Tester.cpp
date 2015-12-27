#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cstdlib>
using namespace std;
typedef long long ll;

const int MAXN = 100000010;
bool vis[MAXN];
int tot, phi[MAXN], prime[MAXN];
void CalPhi(int n) {
	if (tot) return;
	cout << "生成素数中..." << endl;
	memset(vis, 0, sizeof(vis)); phi[1] = 1; tot = 0;
	for (int i = 2; i < n; i++) {
		if (!vis[i]) {
			prime[tot++] = i;
			phi[i] = i - 1;
		}
		for (int j = 0; j < tot; j++) {
			if (i * prime[j] > n) break;
			vis[i * prime[j]] = 1;
			if (i % prime[j] == 0) {
				phi[i * prime[j]] = phi[i] * prime[j];
				break;
			}
			else phi[i * prime[j]] = phi[i] * (prime[j] - 1);
		}
	}
}

bool IsPrime(ll p) {
	if (p < 2) return 0;
	ll end = sqrt(p) + 1;
	for (int i = 2; i < end; i++)
		if (p % i == 0) return 0;
	return 1;
}

void exgcd(ll a, ll b, ll & d, ll & x, ll & y) {
	if (!b) {d = a; x = 1; y = 0;}
	else {exgcd(b, a % b, d, y, x); y -= x * (a / b);}
}

ll inv(ll a, ll m) {
	ll x, y, d;
	exgcd(a, m, d, x, y);
	return d == 1 ? (x + m) % m : -1;
}

ll Euler(ll n) {
	ll rt = n;
	for (int i = 2; i * i <= n; i++)
		if (n % i == 0) {
			rt -= rt / i;
			while (n % i == 0) n /= i;
		}
	if (n > 1) rt -= rt / n;
	return rt;
}

ll Mul(ll a, ll b, ll mod) {
	ll rt = 0;
	while (b) {
		if (b & 1) rt = (rt + a) % mod;
		a = (a << 1) % mod;
		b >>= 1;
	}
	return rt;
}

ll Pow(ll a, ll n, ll mod) {
	ll rt = 1;
	while (n) {
		if (n & 1) rt = Mul(rt, a, mod);
		a = Mul(a, a, mod);
		n >>= 1;
	}
	return rt;
}

ll CalEncrypt(ll phi) {
	ll e;
	while (cout << "是否选择加密密钥[") {
		ll start = rand() % phi;
		for (e = start; e < phi; e++)
			if (__gcd(phi, e) == 1) {
				cout << e << "] (yes/no) ";
				string op; cin >> op;
				if (op == "no") break;
				return e;
			}
	}
	return e;
}

bool FindFactor(ll n, ll & p, ll & q) {
	ll nn = n;
	bool flag = 1;
	for (ll i = 2; n != 1 && i < nn; i++) {
		if (n % i == 0) {
			if (flag == 1) {
				n /= i;
				cout << nn << "=" << i;
				p = i;
				flag = 0;
			}
			else {
				while (n % i == 0) {
					n /= i;
					cout << "*" << i;
				}
			}
		}
	}
	q = nn / p;
	if (IsPrime(p) && (nn % p == 0) && IsPrime(q)) {
		cout << " 破解成功!" << endl;
		return 1;
	}
	return 0;
}

void Encrypt() {
	ll p, q;
	int op;
	cout << "[1]自动生成素数\n[2]手动输入素数 -> "; cin >> op;
	if (op == 2) {
		while (cout << "请输入两个素数: ") {
			cin >> p >> q;
			if (!IsPrime(p)) {
				cout << p << "不是素数" << endl;
				continue;
			}
			if (!IsPrime(q)) {
				cout << q << "不是素数" << endl;
				continue;
			}
			if (p * q < 128) {
				cout << "素数太小" << endl;
				continue;
			}
			break;
		}
	}
	else {
		CalPhi(100000000);
		while ((p = prime[rand() % tot]) && (q = prime[rand() % tot])) {
			if (p == q) continue;
			cout << "是否选择素数[" << p << "]和[" << q << "] (yes/no) ";
			string op; cin >> op;
			if (op == "yes") break;
		}
	}
	ll n = p * q, phi = Euler(n);
	ll e = CalEncrypt(phi), d = inv(e, phi);
	cout << "[公　　钥] " << n << endl;
	cout << "[加密密钥] " << e << endl;
	cout << "[解密密钥] " << d << endl;
	cout << "请输入需要加密的数据: ";
	string m; cin.get(); getline(cin, m);
	cout << "[明文] " << m << endl;
	cout << "[密文] ";
	for (int i = 0; i < (int)m.size(); i++)
		cout << Pow(m[i], e, n) << " ";
	cout << endl;
}

void Decrypt() {
	ll n, d;
	cout << "请输入公钥: "; cin >> n;
	cout << "请输入解密密钥: "; cin >> d;
	ll m; int num;
	cout << "请输入需要解密的数据个数: "; cin >> num;
	cout << "请输入需要解密的数据: ";
	string text;
	while (num--) {
		cin >> m;
		text += (char)Pow(m, d, n);
	}
	cout << "[解密完成] " << text << endl;
}

void Hack() {
	ll n, e;
	cout << "请输入公钥: "; cin >> n;
	cout << "请输入加密密钥: "; cin >> e;
	cout << "分解公钥中...";
	ll p, q;
	if (!FindFactor(n, p, q)) {
		cout << "非法的公钥" << endl;
		return;
	}
	else {
		ll phi = (p - 1) * (q - 1);
		ll d = inv(e, phi);
		cout << "[解密密钥] " << d << endl;
		ll m; int num;
		cout << "请输入需要解密的数据个数: "; cin >> num;
		cout << "请输入需要解密的数据: ";
		string text;
		while (num--) {
			cin >> m;
			text += (char)Pow(m, d, n);
		}
		cout << "[解密完成] " << text << endl;
	}
}

int main() {

	cout << "****************************************" 			<< endl;
	cout << "* RSA Encrypt & Decrypt & Hack" 					<< endl;
	cout << "* Author: mangogao" 								<< endl;
	cout << "* Source: https://github.com/Lodour/RSA-Tester" 	<< endl;
	cout << "****************************************" 			<< endl;

	cout << "[1] RSA加密" << endl;
	cout << "[2] RSA解密" << endl;
	cout << "[3] RSA破解" << endl;
	cout << "[0] 退出" << endl;
	int op;
	srand(time(0));
	while (cout << "请选择功能：" && cin >> op) {
		switch (op) {
		case 0: return 0;
		case 1: Encrypt(); break;
		case 2: Decrypt(); break;
		case 3: Hack(); break;
		}
		cout << endl;
	}
	return 0;
}
