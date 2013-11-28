template<class T>
void d3d_safe_release(T* obj)
{
    if(obj)
    {
        obj->Release();
        obj = 0;
    }
}
