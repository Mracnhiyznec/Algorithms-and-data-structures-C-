#include<bits/stdc++.h>
using namespace std;
const double PI = 3.14159265358979323846;
const double EPS = 10e-6;
class Point {

public:
    double x;
    double y;

    Point(double x = 0, double y = 0) : x(x), y(y) {};

    bool operator == (Point a)
    {
        if (abs(this->x - a.x) < EPS && abs(this->y - a.y) < EPS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator != (Point a)
    {
        return !(*this == a);
    }

    Point operator + (Point B) {
        Point res;
        res.x = x + B.x;
        res.y = y + B.y;
        return res;
    }
    Point operator - (Point B) {
        Point res;
        res.x = x - B.x;
        res.y = y - B.y;
        return res;
    }
    Point operator * (double k) {
        Point res;
        res.x = x * k;
        res.y = y * k;
        return res;
    }

    int operator * (Point k)
    {
        return this->x * k.x + this->y * k.y;
    }
};


// inline â ñèëó òîãî, ÷òî ýòà ô-öèÿ êàê áû ãëîáàëüíàÿ äëÿ âñåõ êòî ïîëóåòñÿ ðàññòîÿíèåì
inline double dist(Point a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}
class Line
{
public:
    double A, B, C;

    // êîíñòðóêòîð ëèíèè ïî äâóì òî÷êàì
    Line(Point p1, Point p2)
    {
        A = p2.y - p1.y;
        B = p1.x - p2.x;
        C = -p1.x * p2.y + p2.x * p1.y;
    }

    // êîíñòóêòîð ëèíèè ïî îáùåìó óð-íèþ ïðÿìîé
    Line(double A, double B, double C) {
        this->A = A;
        this->B = B;
        this->C = C;
    }

    // êîíñòóêòîð ëèíèè ïî òî÷êå è óãëó íàêëîíà
    Line(double coefficient, Point p1)
    {
        Point p2;
        double shift;
        shift = p1.y - p1.x * coefficient;

        if (p1.x != -1)
        {
            p2.x = p1.x + 1;
        }
        else {
            p2.x = p1.x - 1;
        }

        p2.y = coefficient * p2.x + shift;

        A = p2.y - p1.y;
        B = p1.x - p2.x;
        C = -p1.x * p2.y + p2.x * p1.y;
    }

    bool operator == (Line a)
    {
        if (abs(this->A - a.A) < EPS && abs(this->B - a.B) < EPS && abs(this->C - a.C) < EPS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator != (Line a)
    {
        return !(a == *this);
    }

    // ô-öèÿ ìåíÿåò ïðåäñòàâëåíèÿ ïðÿìîé îò îáùåãî âèäà ê âåêòîðíîìó
    /*
    Ax+By+C = 0 ==> r = at+b
    */
    pair<Point, Point> transform_equation_line()
    {
        // òî÷êè - îíè æå âåêòîðà
        Point a, b;
        if (abs(this->A) < EPS) {
            a = Point(0, -this->C / this->B);
            b = Point(1, -this->C / this->B);
        } 
        else
        {
            a = Point(-this->C / this->A, 0);
            b = Point(-(this->C + this->B) / A, 1);
        }
        pair<Point, Point> ans;
        ans.first = a;
        ans.second = b;

        return ans;
    } 
};
class Shape
{

public:
    double perimetr();

    double area();

    // ñîâïàäàåò ëè ôèãóðà ñ äàííîé
    bool operator == (Shape& another);

    //// ðàâíà ëè ýòà ôèãóðà â ãåîìåòðè÷åñêîì ñìûñëå
     bool isCongruentTo(const Shape& another);

    //// ïîäîáíà ëè ýòà ôèãóðà â ãåîìåòðè÷åñêîì ñìûñëå
     bool isSimilarTo(const Shape& another);

    //// ñîäåðæèò ëè ôèãóðà òî÷êó
     bool containsPoint(Point point);


     void rotate(Point center, double angle);

    //// ñèììåòðèÿ îòíîñèòåëüíî ëèíèè
     void reflex(Line axis);

    //// ñèììåòðèÿ îòíîñèòåëüíî òî÷êè
     void reflex(Point center);

     void scale(Point center, double coef);
};
class Polygon : public Shape
{
public:

    // ïîëó÷àåì count òî÷åê. èíèöèàëèçèðóåì ôèãóðó
    Polygon(int count, ...)
    {
        this->count_vertex = count;

        // Ìû ïîëó÷àåì äîñòóï ê ìíîãîòî÷èþ ÷åðåç va_list, ïîýòîìó äàâàéòå îáúÿâèì åãî
        va_list list;

        // Ìû èíèöèàëèçèðóåì va_list ñ ïîìîùüþ va_start. Ïåðâûé ïàðàìåòð
        // - ñïèñîê äëÿ èíèöèàëèçàöèè. Âòîðîé ïàðàìåòð - ýòî ïîñëåäíèé
        // ïàðàìåòð ïåðåä ìíîãîòî÷èåì.
        va_start(list, count);

        // Ïåðåáèðàåì âñå àðãóìåíòû ìíîãîòî÷èÿ
        for (int arg = 0; arg < count; ++arg)
        {
            // Ìû èñïîëüçóåì va_arg äëÿ ïîëó÷åíèÿ ïàðàìåòðîâ èç íàøåãî ìíîãîòî÷èÿ
            // Ïåðâûé ïàðàìåòð - ýòî va_list, êîòîðûé ìû èñïîëüçóåì
            // Âòîðîé ïàðàìåòð - ýòî òèï ïàðàìåòðà
            this->vertexs.push_back(va_arg(list, Point));
        }

        // Î÷èùàåì va_list, êîãäà çàêîí÷èëè.
        va_end(list);
    }



    // ñêàëÿðíîå ïðîèçâåäåíèå
    double dot(Point A, Point B)
    {
        return A.x * B.x + A.y * B.y;
    }

    // âåêòîðíîå ïðîèçâåäåíèå
    double cross(Point A, Point B)
    {
        return A.x * B.y - A.y * B.x;
    }


    // ô-öèÿ ïîäñ÷åòà ïëîùàäè ìíîãîóãîëüíèêà
    double area() 
    {
        double res = 0;
        for (int i = 0; i < vertexs.size(); i++)
        {
            Point
                p1 = i ? vertexs[i - 1] : vertexs.back(),
                p2 = vertexs[i];
            res += (p1.x - p2.x) * (p1.y + p2.y);
        }
        return fabs(res) / 2;
    }

    double perimetr() 
    {
        double sum = 0;
        for (int i = 0; i < vertexs.size(); i++)
        {
            Point
                p1 = i ? vertexs[i - 1] : vertexs.back(),
                p2 = vertexs[i];
            Line cur(p1, p2);

            // êîñòûëüíî, íî ñ÷èòàåì ïåðèìåòð ÷åðåç êâàäðàò ðàññòîÿíèÿ ìåæäó òî÷êàìè
            double square_distance_p1_p2 = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
            sum += sqrt(square_distance_p1_p2);
        }
        return sum;
    }

    // êîíñòðóêòîð ìíîãîóãîëüíèêà. Ìèíèìàëüíûé ìíîãîóãîëüíèê (íà ïë-òè) òðåóãîëüíèê
    Polygon(vector<Point> vertexs)
    {
        this->count_vertex = vertexs.size();
        this->vertexs = vertexs;
    }

    // çíàê ðàñïîëîæåíèÿ òî÷êè îòíîñèòåëüíî ïðÿìîé
    int sign(Line L, Point p) {
        double eps = 1e-6;
        if (abs(L.A * p.x + L.B * p.y + L.C) >= 0) {
            return 1;
        }
        else {
            return -1;
        }
    }

    // ô-öèÿ ïðîâåðêè âûïóêëûé ëè ìíîãîóãîëüíèê èëè íåò
    bool isConvex()
    {
        bool flag_convex = true;
        // ñ÷èòàåì êîëè÷åñòâî âåðøèí êîòîðûå ðàñïîëîæåíû ïî ðàçíûå ñòîðîíû ëèíèè
        // åñëè êîëè÷åñòâî ðàâíî èñõîäíîìó, òî âñå âåðøèíû ïî îäíó ñòîðîíó è ìíîãîóã, âûïóêëûé
        int count_vertex_line = 0;

        for (int i = 0; i < vertexs.size(); i++)
        {
            Point
                p1 = i ? vertexs[i - 1] : vertexs.back(),
                p2 = vertexs[i];

            Line egle(p1, p2);

            for (int i = 0; i < vertexs.size(); i++)
            {
                count_vertex_line += sign(egle, vertexs[i]);
            }

            if (count_vertex_line == vertexs.size())
            {
                count_vertex_line = 0;
                // âåñü ìíîãîóã, ïî îäíó ñòîðîíó îò ïðÿìîé, ïðîâåðÿåì äàëüøå
                continue;
            }
            else
            {
                // ïîéìàëè ñèòóàöèþ êîãäà íå âñå âåðøèíû ïî îäíó ñòîðîíó îò ïðÿìîé çíà÷èò ïðÿìîóã íå âûïóêëûé
                flag_convex = false;
                return flag_convex;
            }
        }
    }

    // ïðîâåðêà íà âûïóêëîñòü ÷åðåç âåêòîðíîå ïðîèçâåäåíèå
    bool isConvex_v2()
    {
        /*
        Ìîæíî ïðîéòèñü ïî ñòîðîíàì ìíîãîóãîëüíèêà è ïðîâåðÿòü âåêòîðíûì ïðîèçâåäåíèåì,
        ÷òî ìû ïîâîðà÷èâàåì âñåãäà â îäíó ñòîðîíó
        */

        double cross_prev = NULL;

        Point a = this->vertexs[0];
        Point b = this->vertexs[1];

        for (int i = 2; i < this->vertexs.size(); i++)
        {
            Point c = this->vertexs[i];

            if (cross_prev == NULL)
            {
                cross_prev = cross(b - a, c - a);
            }

            if (cross(b - a, c - a) * cross_prev > 0)
            {
                continue;
            }
            else
            {
                return false;
            }
            a = b;
            b = c;
        }
        return true;
    }

    vector<Point> getVertex()
    {
        return this->vertexs;
    }


    int getCountVertex()
    {
        return count_vertex;
    }

    // ô-öèÿ îáõîäà ôèãóðû. ñ÷èòàåò å¸ óãëû è äëèííû ñòîðîí. çàïèñûâàåò â ìàññèâû ñîîòâ
    void round_polygon(vector<Point>& vertexs, vector<double>& Angles, vector<double>& Sides)
    {
        Point prev1 = NULL;
        Point prev2 = NULL;

        for (int i = 0; i < vertexs.size(); i++)
        {
            double angle = 0;
            Point p1;
            Point p2;
            Point A;
            Point B;

            if (i == vertexs.size() - 1)
            {
                p1 = vertexs[i];
                p2 = vertexs[0];
            }
            else
            {
                p1 = vertexs[i];
                p2 = vertexs[i + 1];
            }

            if (prev1 == NULL && prev2 == NULL)
            {
                prev1 = p1;
                prev2 = p2;
            }
            else
            {
                Point A = p2 - p1;
                Point B = prev1 - prev2;
                angle = dot(A, B) / (sqrt(A.x * A.x + A.y * A.y) * sqrt(B.x * B.x + B.y * B.y));

                cout << angle << '\n';
                
                Angles.push_back(angle);

                prev1 = p1;
                prev2 = p2;
            }

            Sides.push_back(dist(p2 - p1));
            cout << p1.x << ' ' << p1.y << '|' << ' ' << p2.x << ' ' << p2.y << '\n';
        }
    }

    // ô-öèÿ ïðîñòîãî âûâîäà â êîíñîëü ñòîðîí è cos óãëîâ òåêóùåé ôèãóðû
    void print_angles_sides(vector<double>& Angles, vector<double>& Sides)
    {
        cout << "óãëû = "; for (auto i : Angles) cout << i << ' ';
        cout << "===============\n";
        cout << "ñòîðîíû = "; for (auto i : Sides) cout << i << ' ';

        cout << '\n';
    }

    // ô-öèÿ îáõîäà óãëîâ è ïðîâåðêè ñîâïàäåíèÿ. åñëè âñå óãëû ñîâïàëè => true
    bool check_equality_angles(vector<double> angle_this, vector<double> angle_another)
    {
        for (int i = 0; i < angle_this.size(); i++)
        {
            // åñëè óãëû îòëè÷àþòñÿ ìàëî, òî îíè ðàâíû
            if (abs(angle_another[i] - angle_this[i]) <= EPS)
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }

    // ô-öèÿ ïðîâåðêè ÷òî ñòîðîíû ïîäîáíû
    // ïîäðàçóìåâàåòñÿ ÷òî ñòîðîí îäèííàêîâîå êîë-âî è îíè íå íóëåâûå
    bool check_koef_figurs(vector<double>& sides_this, vector<double>& sides_another)
    {
        double prev_koef = sides_this[0] / sides_another[0];

        for (int i = 1; i < sides_this.size(); i++)
        {
            double cur_koef = sides_this[i] / sides_another[i];

            if (cur_koef == prev_koef)
            {
                continue;
            }
            else
            {
                false;
            }
        }
        return true;
    }

    // ñðàâíåíèå â ãåîìåòðè÷åñêîì ñìûñëå
    bool isCongruentTo(Polygon& another)
    {
        vector<double> sides_this;
        vector<double> sides_another;

        vector<double> angle_this;
        vector<double> angle_another;

        vector<Point> vertexs_another = another.getVertex();
        // åñëè íå ñîâïàëè êîëè÷åñòâî âåðøèí, òî òî÷íÿê ðàçíûå ôèãóðû
        if (this->getCountVertex() != another.getCountVertex())
        {
            return false;
        }

        // îáõîäèì äâå ôèãóðû ïî êîíòðóðó, çàïèñûâàåì äëèííû ñòîðîí è êîñèíóñû óãëîâ;
        round_polygon(this->vertexs, angle_this, sides_this);
        cout << "===========\n";
        round_polygon(another.vertexs, angle_another, sides_another);

        print_angles_sides(angle_this, sides_this);
        print_angles_sides(angle_another, sides_another);

        // ïðîâåðèì ñîâïàäåíèå óãëîâ ôèãóð
        if (check_equality_angles(angle_this, angle_another))
        {
            // óãëû ñîâïàëè, ïðîâåðèì ïîäîáèå ñòîðîí
            if (check_koef_figurs(sides_this, sides_another))
            {
                // åñëè ñòîðîíû âñå ïîäîáíû, ïðîâåðèì êîýô ïîäîáèÿ
                double koef = sides_this[0] / sides_another[0];

                if (abs(koef - 1) <= EPS)
                {
                    // äà, ôèãóðû ïîäîáíû ñ êîýô ïîäîáèÿ 1
                    return true;
                }
                else
                {
                    return false;
                }

            }
            else
            {
                return false;
            }
        }
        else
        {
            // óãëû íå âñå ñîâïàëè, ôèãóðêè óæå òî÷íÿê íå ðàâíû
            return false;
        }
    }

    // îïåðàòîð ñðàâíåíèÿ 
    bool operator == (Polygon& another)
    {
        if (this->isCongruentTo(another))
        {
            return true;
        }
        return false;

    }


    bool isSimilarTo(Polygon& another)
    {
        vector<double> sides_this;
        vector<double> sides_another;

        vector<double> angle_this;
        vector<double> angle_another;

        // îáõîäèì äâå ôèãóðû ïî êîíòðóðó, çàïèñûâàåì äëèííû ñòîðîí è êîñèíóñû óãëîâ;
        round_polygon(this->vertexs, angle_this, sides_this);
        cout << "===========\n";
        round_polygon(another.vertexs, angle_another, sides_another);

        // ïðîâåðèì ñîâïàäåíèå óãëîâ ôèãóð
        if (check_equality_angles(angle_this, angle_another))
        {
            // óãëû ñîâïàëè, ïðîâåðèì ïîäîáèå ñòîðîí
            if (check_koef_figurs(sides_this, sides_another))
            {
                return true;

            }
            else
            {
                return false;
            }
        }
        else
        {
            // óãëû íå âñå ñîâïàëè, ôèãóðêè óæå òî÷íÿê íå ïîäîáíû
            return false;
        }
    }


    // ïðîâåðêà ÷òî òî÷êà âíóòðè ôèãóðû
    bool containsPoint(Point point)
    {
        /*
        Ïåðâàÿ ñòðîêà óñëîâèÿ ïðîâåðÿåò ïîïàäàíèå point.Y ìåæäó çíà÷åíèÿìè p[i].Y è p[j].Y, 
        êîíòðîëèðóåò íàïðàâëåíèå ïðîõîäà âåðøèíû è îáåñïå÷èâàåò íåíóëåâîé çíàìåíàòåëü îñíîâíîé ôîðìóëû.
        Âòîðàÿ ñòðîêà ïðîâåðÿåò íàõîæäåíèå ñòîðîíû p[i]p[j] ñëåâà îò òî÷êè point.
        Òðåòüÿ ñòðîêà ôîðìèðóåò îòðèöàòåëüíûé îòâåò ïðè ÷¸òíîì êîëè÷åñòâå ñòîðîí ñëåâà 
        è ïîëîæèòåëüíûé — ïðè íå÷¸òíîì.
        */
        bool result = false;
        int j = this->vertexs.size() - 1;
        for (int i = 0; i < this->vertexs.size(); i++) {
            if ((this->vertexs[i].y < point.y && this->vertexs[j].y >= point.y || this->vertexs[j].y < point.y && this->vertexs[i].y >= point.y) &&
                (this->vertexs[i].x + (point.y - this->vertexs[i].y) / (this->vertexs[j].y - this->vertexs[i].y) * (this->vertexs[j].x - this->vertexs[i].x) < point.x))
                result = !result;
            j = i;
        }

        return result;
    }



    // ïîâîðîò ëþáîé ôèãóðû îòíîñèòåëüíî öåíòðà íà óãîë
    void rotate(Point center, double angle)
    {
        double dx = center.x;
        double dy = center.y;

        for (int i = 0; i < this->vertexs.size(); i++)
        {
            int x = this->vertexs[i].x;
            int y = this->vertexs[i].y;
            this->vertexs[i].x = ((x - dx) * cos(angle)) - ((y - dy) * sin(angle)) + dx;
            this->vertexs[i].y = ((x - dx) * sin(angle)) + ((y - dy) * cos(angle)) + dy;
        }
    }

    // ñèììåòðèÿ îòíîñèòåëüíî òî÷êè
    void reflex(Point center)
    {
        for (int i = 0; i < this->vertexs.size(); i++)
        {
            Point cur = this->vertexs[i];
            Point direction_center = center - cur;
            double dist_cur_and_center = dist(direction_center);

            direction_center.x = direction_center.x / dist_cur_and_center;
            direction_center.y = direction_center.y / dist_cur_and_center;

            this->vertexs[i] = center + (direction_center * dist_cur_and_center);

        }
    }


    // ñèììåòðèÿ îòíîñèòåëüíî ïðÿìîé
    void reflex(Line axis)
    {
        pair<Point, Point> new_view_axis = axis.transform_equation_line();
        Point a = new_view_axis.first;
        Point b = new_view_axis.second; 

        for (int i = 0; i < this->vertexs.size(); i++)
        {
            double x = vertexs[i].x;
            double y = vertexs[i].y;

            // ïðîâåðêà ÷òî òî÷êà êîòîðóþ áóäåò îòîáðàæàòü ëåæèòü íà äàííîé ïðÿìîé
            if (abs(axis.A * x + axis.B * y + axis.C) < EPS)
            {
                continue;
            }
            else
            {
                /*
                Ñåìàíòèêà ôîðìóëû òàêîâà - âû÷èñëèòü ðàññòîÿíèÿ îò òî÷êè äî ïðÿìîé,
                è ýòî ðàññòîÿíèÿ îòëîæèòü, íî â ïðîòèâîïîëîæíóþ ñòîðîíó
                */

                Point ps = this->vertexs[i];

                Point n;
                n.x = a.y - b.y;
                n.y = b.x - a.x;

                double len = sqrt(n.x * n.x + n.y * n.y);
                n.x /= len;
                n.y /= len;

                double dot2 = 2 * (n.x * ps.x + n.y * ps.y);

                this->vertexs[i].x = ps.x - dot2 * n.x;
                this->vertexs[i].y = ps.y - dot2 * n.y;
            }
        }
    }


    void scale(Point center, double coef)
    {
        for (int i = 0; i < vertexs.size(); i++)
        {
            vertexs[i].x = center.x + (vertexs[i].x - center.x) * coef;
            vertexs[i].y = center.y + (vertexs[i].y - center.y) * coef;
        }
    }



private:
    int count_vertex = 0;

    vector<Point> vertexs;

};
class Ellipse: public Shape
{
public:

    Ellipse(Point focus_first, Point focus_second, double distantion) {
        
        this->focus_first = focus_first;
        this->focus_second = focus_second;
        this->distantion = distantion;

        //Äëÿ òîãî ÷òîáû òî÷êà ëåæàëà íà ýëëèïñå, íåîáõîäèìî è äîñòàòî÷íî,
        //÷òîáû ñóììà åå ðàññòîÿíèé äî ôîêóñîâ ðàâíÿëàñü áîëüøîé îñè ýëëèïñà 2a
        this->a = distantion / 2;

        this->c = sqrt(dist(focus_first - focus_second));

        this->eccentricity = this->c / this->a;

        this->b = sqrt(this->a * this->a - this->c * this->c);

        this->center.x = (focus_first.x + focus_second.x) / 2;
        this->center.y = (focus_first.y + focus_second.y) / 2;

    }

    pair<Point, Point> focuses()
    {
        return { focus_first, focus_second };
    }

    pair<Line, Line> directrices()
    {
        return { Line(eccentricity, 0, a), Line(eccentricity, 0, -a) };
    }

    double getEccentricity()
    {
        return this->eccentricity;
    }

    Point getCenter()
    {
        return this->center;
    }

    double perimetr() 
    {
        return 2 * PI * sqrt((a * a + b * b) / 2);
    }

    double area() 
    {
        return PI * a * b;
    }


    // ðàâåíñòâî äâóõ ýëëèïñîâ
    bool isCongruentTo(Ellipse& another)
    {
        if (abs((a / another.a) - (b / another.b)) <= EPS)
        {
            if (abs(a / another.a - 1) <= EPS)
            {
                return true;
            }
        }
        return false;
    }

    bool containsPoint(Point point)
    {
        if (dist(focus_first - point) + dist(focus_second - point) <= 2 * a)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator == (Ellipse& another)
    {
        if (this->isCongruentTo(another))
        {
            return true;
        }
        return false;
    }

    // ïîäîáèå äâóõ
    bool isSimilarTo(Ellipse& another)
    {
        if (abs((a / another.a) - (b / another.b)) <= EPS)
        {
            return true;
        }
        return false;
    }


    // ïîâîðîò ëþáîé ôèãóðû îòíîñèòåëüíî öåíòðà íà óãîë
    void rotate(Point center, double angle)
    {
        double dx = center.x;
        double dy = center.y;
        int x = focus_first.x;
        int y = focus_first.y;

        focus_first.x = ((x - dx) * cos(angle)) - ((y - dy) * sin(angle)) + dx;
        focus_first.y = ((x - dx) * sin(angle)) + ((y - dy) * cos(angle)) + dy;

        x = focus_second.x;
        y = focus_second.y;
        focus_second.x = ((x - dx) * cos(angle)) - ((y - dy) * sin(angle)) + dx;
        focus_second.y = ((x - dx) * sin(angle)) + ((y - dy) * cos(angle)) + dy;

        this->center.x = (focus_first.x + focus_second.x) / 2;
        this->center.y = (focus_first.y + focus_second.y) / 2;


    }

    // ñèììåòðèÿ îòíîñèòåëüíî òî÷êè
    void reflex(Point center)
    {
        // äëÿ ïåðâîãî è âòîðîãî ôîêóñà ñîîòâ
            Point cur = focus_first;
            Point direction_center = center - cur;
            double dist_cur_and_center = dist(direction_center);

            direction_center.x = direction_center.x / dist_cur_and_center;
            direction_center.y = direction_center.y / dist_cur_and_center;

            focus_first = center + (direction_center * dist_cur_and_center);

            cur = focus_second;
            direction_center = center - cur;
            dist_cur_and_center = dist(direction_center);

            direction_center.x = direction_center.x / dist_cur_and_center;
            direction_center.y = direction_center.y / dist_cur_and_center;

            focus_second = center + (direction_center * dist_cur_and_center);

            this->center.x = (focus_first.x + focus_second.x) / 2;
            this->center.y = (focus_first.y + focus_second.y) / 2;
    }

    // ñèììåòðèÿ îòíîñèòåëüíî ïðÿìîé
    void reflex(Line axis)
    {
        pair<Point, Point> new_view_axis = axis.transform_equation_line();
        Point a = new_view_axis.first;
        Point b = new_view_axis.second;

        double x = focus_first.x;
        double y = focus_first.y;

        // ïðîâåðêà ÷òî òî÷êà êîòîðóþ áóäåò îòîáðàæàòü ëåæèòü íà äàííîé ïðÿìîé
        if (abs(axis.A * x + axis.B * y + axis.C) < EPS)
        {
        }
        else
        {
            /*
            Ñåìàíòèêà ôîðìóëû òàêîâà - âû÷èñëèòü ðàññòîÿíèÿ îò òî÷êè äî ïðÿìîé,
            è ýòî ðàññòîÿíèÿ îòëîæèòü, íî â ïðîòèâîïîëîæíóþ ñòîðîíó
            */

            Point ps = focus_first;

            Point n;
            n.x = a.y - b.y;
            n.y = b.x - a.x;

            double len = sqrt(n.x * n.x + n.y * n.y);
            n.x /= len;
            n.y /= len;

            double dot2 = 2 * (n.x * ps.x + n.y * ps.y);

            focus_first.x = ps.x - dot2 * n.x;
            focus_first.y = ps.y - dot2 * n.y;

            ps = focus_second;

            n;
            n.x = a.y - b.y;
            n.y = b.x - a.x;

            len = sqrt(n.x * n.x + n.y * n.y);
            n.x /= len;
            n.y /= len;

            dot2 = 2 * (n.x * ps.x + n.y * ps.y);

            focus_second.x = ps.x - dot2 * n.x;
            focus_second.y = ps.y - dot2 * n.y;

            this->center.x = (focus_first.x + focus_second.x) / 2;
            this->center.y = (focus_first.y + focus_second.y) / 2;
        }
    }



    void scale(Point center, double coef)
    {
        focus_first.x = center.x + (focus_first.x - center.x) * coef;
        focus_first.y = center.y + (focus_first.y - center.y) * coef;
        focus_second.x = center.x + (focus_second.x - center.x) * coef;
        focus_second.y = center.y + (focus_second.y - center.y) * coef;

        this->center.x = (focus_first.x + focus_second.x) / 2;
        this->center.y = (focus_first.y + focus_second.y) / 2;
    }


private:

    double a, b, c, eccentricity;
    double distantion;
    Point focus_first;
    Point focus_second;
    Point center;

    // âåêòîð íà êîòîðûé áóäåò ñäâèã ñèñòåìû êîîðä
    Point shift;
    // åñëè ó íàñ ýëëèïñ 'ñòîèò' à íå ëåæèò íà îñè Îõ
    bool flag_rotate = false;

};
class Rectangle: public Polygon
{
public:
    using Polygon::Polygon; // ïîäêëþ÷àåì êîíñòðóêòîð áàçîâîãî êëàññà

    Rectangle(Point bottom_left, Point top_right) : Polygon(4, bottom_left, Point(top_right.x, bottom_left.y), top_right, Point(bottom_left.x, top_right.y)) 
    {
        this->bottom_left = bottom_left;
        this->bottom_right = Point(top_right.x, bottom_left.y);
        this->top_right = top_right;
        this->top_left = Point(bottom_left.x, top_right.y);
    };

    pair<Line, Line> getDiagonals()
    {
        Line first_diagonal(bottom_left, top_right);
        Line second_diagonal(bottom_right, top_left);

        return { first_diagonal, second_diagonal };
    }


    Point getCenter()
    {
        double center_x = (bottom_left.x + bottom_right.x) / 2;
        double center_y = (bottom_left.y + top_left.y) / 2;

        return { center_x, center_y };
    }

    Point center;
    Point bottom_left;
    Point bottom_right;
    Point top_right;
    Point top_left;

};
class Circle: public Ellipse
{
public:
    Circle(Point center, double radius) : Ellipse(center, center, radius)
    {
        this->center = center;
        this->radius = radius;
    }
    
    double getRadius()
    {
        return this->radius;
    }

    // ��������� ���� �����������
    bool isCongruentTo(Circle& another)
    {
        if (abs(this->radius - another.radius) < EPS)
        {
            return true;
        }
        return false;
    }

    bool isSimilarTo(Circle& another)
    {
        return true;
    }

    bool operator == (Circle& another)
    {
        if (this->isCongruentTo(another))
        {
            return true;
        }
        return false;
    }
private:
    Point center;
    double radius;

};
class Triangle :
    public Polygon
{
public:

    Triangle(Point a, Point b, Point c) : Polygon(3, a, b, c) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->dist_ab = dist(a - b);
        this->dist_bc = dist(b - c);
        this->dist_ac = dist(a - c);
    };

    
    // îïèñàííàÿ îêðóæíîñòü
    Circle getCircumscribedCircle()
    {
        double radius_described = (dist_ab * dist_ac * dist_bc) / 4 * this->area();
        double x1 = a.x; double y1 = a.y; double x2 = b.x; double y2 = b.y; double x3 = c.x; double y3 = c.y;
        double d = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
        double xp = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / d;
        double yp = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / d;

        Point center(xp, yp);
        return Circle(center, radius_described);

    }

    Circle getInscribedScribedCircle()
    {
        double x1 = a.x; double y1 = a.y; double x2 = b.x; double y2 = b.y; double x3 = c.x; double y3 = c.y;
        double radius = (2 * area()) / (dist_ab + dist_ac + dist_bc);

        double perimert_half = (dist_ab + dist_ac + dist_bc) / 2;

        double x0 = ((x1 + x2 - 2 * x3) * ((y3 - y1) * (x2 + x3 - 2 * x1) + x1 * (y2 + y3 - 2 * y1)) - x3 * (x2 + x3 - 2 * x1) * (y1 + y2 - 2 * y3)) / ((y2 + y3 - 2 * y1) * (x1 + x2 - 2 * x3) - (x2 + x3 - 2 * x1) * (y1 + y2 - 2 * y3));
        double y0 = ((x0 - x1) * (y2 + y3 - 2 * y1) / (x2 + x3 - 2 * x1)) + y1;

        Point center(x0, y0);

        return Circle(center, radius);
    }

    // öåíòð ìàññ
    Point getCentroid()
    {
        Point res((a + b + c));

        res = res * (1/3);

        return res;
    }

    Point findOrthocenter()
    {
        Point centroid = getCentroid();
        Circle cicle_triangle = getCircumscribedCircle();
        Point circumcenter = cicle_triangle.getCenter();
        Point h = { (3 * centroid.x - 2 * circumcenter.x), (3 * centroid.y - 2 * circumcenter.y) };

        return h;
    }


    Line EulerLine()
    {
        Circle first_circle = getCircumscribedCircle();
        Circle second_circle = getInscribedScribedCircle();

        Point first = first_circle.getCenter();
        Point second = second_circle.getCenter();

        return Line(first, second);
    }


    Circle ninePointCircle()
    {
        Circle opisannaya = getCircumscribedCircle();

        double raduis_opisannaya = opisannaya.getRadius();

        double radiusNIEN = raduis_opisannaya / 2;


        Point center = opisannaya.getCenter();

        Point orto_center = findOrthocenter();

        Point NIEN_center = (center + orto_center) * 0.5;

        return Circle(NIEN_center, radiusNIEN);
    }

private:
    Point a;
    Point b;
    Point c;
    double dist_ab = dist(a - b);
    double dist_bc = dist(b - c);
    double dist_ac = dist(a - c);
};


int main(){

    return 0;
}
