//  ����̽��:
//  h(k, i) = (h2(k) + c1 * i + c2 * i * i)
//  c1 = 1 / 2, c2 = 1 / 2.
//  �㷨��Ҫ�ܱ�����m����λ�е�ÿһ��. ��ô, ����p, q(p != q),
//  ��j < mʱ, h(k, p) != h(k, q). 
//  �������ܱ�֤, ÿ����λ���ܱ�̽�鵽,
//  ��j == mʱ, ̽�����, ���еĲ�λ����̽�鵽.
//  ����h(k, p) == h(k, q), ��ôp(p+1) / 2 === q(q+1) / 2 (mod m). ��:
//		(p^2 + p - q^2 - q) / 2 === 0 (mod m)
//  =>	(p - q)(p + q + 1) === 0 (mod 2m)	�����2�ŵ���mod����.
//	��֪m = 2^k, ��:
//	=>	(p - q)(p + q + 1) = x * 2 * 2^k = x * 2^(k+1).
//	����������ߵ���ʽ����ż��.
//  p-q��p+q+1��Ȼ����ż�����. Ҳ����˵, ����2^(k+1)������(��2m), ��Ȼ��
//  p-q����p+q+1һ����ʽ�ṩ.
//  ��:
//  2^k | p-q		(1)
//  2^k | p+q+1		(2)
//  ��Ȼ���ҽ���һ������.
//  �ȿ���(1), ��ȻС��m, ����(1)�����ܳ���.
//  �ٿ���(2), p+q��Ȼ <= (m-1 + m-2), ����(2) <= m + m - 2.
//  Ҳ��С��2m, ���, h(k, p) == h(k, q)�Ǽ�����, ���, h(k, p) != h(k, q).
//